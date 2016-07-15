#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libnotify/notify.h>

extern int rx, ry, rw, rh;

int main(int argc, int *argv[])
{
  pid_t rectangle_child_pid, rectangle_pid;
  pid_t byzanz_child_pid, byzanz_pid;
  int rectangle_status;
  int byzanz_status;
  int rx = 0, ry = 0, rw = 0, rh = 0;
  int rect_x = 0, rect_y = 0, rect_w = 0, rect_h = 0;
  int btn_pressed = 0, done = 0;

  notify_init ("Screen Recording Notification");
  NotifyNotification * ScreenRecording = notify_notification_new ("Screen Recording", "Screen Recording has started...", "dialog-information");
  notify_notification_set_timeout (ScreenRecording, 0);

  XEvent ev;
  Display *disp = XOpenDisplay(NULL);

  if(!disp)
    return EXIT_FAILURE;

  Screen *scr = NULL;
  scr = ScreenOfDisplay(disp, DefaultScreen(disp));

  Window root = 0;
  root = RootWindow(disp, XScreenNumberOfScreen(scr));

  Cursor cursor, cursor2;
  cursor = XCreateFontCursor(disp, XC_left_ptr);
  cursor2 = XCreateFontCursor(disp, XC_lr_angle);

  XGCValues gcval;
  gcval.foreground = XWhitePixel(disp, 0);
  gcval.function = GXxor;
  gcval.background = XBlackPixel(disp, 0);
  gcval.plane_mask = gcval.background ^ gcval.foreground;
  gcval.subwindow_mode = IncludeInferiors;

  GC gc;
  gc = XCreateGC(disp, root,
                 GCFunction | GCForeground | GCBackground | GCSubwindowMode,
                 &gcval);

  if (argc < 2) {
    printf("You must pass a duration for the recording.\n");
    return EXIT_FAILURE;
  }

  /* this XGrab* stuff makes XPending true ? */
  if ((XGrabPointer
       (disp, root, False,
        ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
        GrabModeAsync, root, cursor, CurrentTime) != GrabSuccess))
    printf("couldn't grab pointer:");

  if ((XGrabKeyboard
       (disp, root, False, GrabModeAsync, GrabModeAsync,
        CurrentTime) != GrabSuccess))
    printf("couldn't grab keyboard:");

  // see also: http://stackoverflow.com/questions/19659486/xpending-cycle-is-making-cpu-100
  while (!done) {
    //~ while (!done && XPending(disp)) {
      //~ XNextEvent(disp, &ev);
    if (!XPending(disp)) { usleep(1000); continue; } // fixes the 100% CPU hog issue in original code
    if ( (XNextEvent(disp, &ev) >= 0) ) {
      switch (ev.type) {
        case MotionNotify:
        /* this case is purely for drawing rect on screen */
          if (btn_pressed) {
            if (rect_w) {
              /* re-draw the last rect to clear it */
              XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
            } else {
              /* Change the cursor to show we're selecting a region */
              XChangeActivePointerGrab(disp,
                                       ButtonMotionMask | ButtonReleaseMask,
                                       cursor2, CurrentTime);
            }
            rect_x = rx;
            rect_y = ry;
            rect_w = ev.xmotion.x - rect_x;
            rect_h = ev.xmotion.y - rect_y;

            if (rect_w < 0) {
              rect_x += rect_w;
              rect_w = 0 - rect_w;
            }
            if (rect_h < 0) {
              rect_y += rect_h;
              rect_h = 0 - rect_h;
            }
            /* draw rectangle */
            XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
            XFlush(disp);
          }
          break;
        case ButtonPress:
          btn_pressed = 1;
          rx = ev.xbutton.x;
          ry = ev.xbutton.y;
          break;
        case ButtonRelease:
          done = 1;
          break;
      }
    }
  }

  rw = ev.xbutton.x - rx;
  rh = ev.xbutton.y - ry;
  /* cursor moves backwards */
  if (rw < 0) {
    rx += rw;
    rw = 0 - rw;
  }
  if (rh < 0) {
    ry += rh;
    rh = 0 - rh;
  }

  /* clear the drawn rectangle */
  if (rect_w) {
    XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
    XFlush(disp);
  }

  XCloseDisplay(disp);

  if (rw > 0 && rh > 0) {
    notify_notification_show (ScreenRecording, NULL);
    byzanz_child_pid = fork();
    if (byzanz_child_pid == 0) {
      byzanz_pid = getpid();

      char buf[128];
      snprintf(buf, sizeof(buf), "byzanz-record -d %s --delay=0 -x %d -y %d -w %d -h %d ~/recording.gif",argv[1],rx,ry,rw,rh);
      system(buf);

      return EXIT_SUCCESS;
    }
  }

  if (byzanz_pid > 0) {
    byzanz_pid = wait(&byzanz_status);

    if ( WIFEXITED(byzanz_status) ) {
      notify_notification_update (ScreenRecording, "Screen Recording", "Screen Recording complete, file saved to ~/recording.gif.", "dialog-information");
      notify_notification_set_timeout (ScreenRecording, NOTIFY_EXPIRES_DEFAULT);
      notify_notification_show (ScreenRecording, NULL);
      g_object_unref(G_OBJECT(ScreenRecording));
      notify_uninit();
    }
  }

  return EXIT_SUCCESS;
}
