#include "../apkenv.h"
#include <fcntl.h>
#include <linux/input.h>
#include <sys/select.h>
#include <sys/time.h>
struct EvdevState {
int fd;
unsigned long id,pid;
int x[10];
int y[10];
int finger; //Last finger id
int pfinger; //Previous finger value,needed to detect ACTION_UP
int state;
int screen_width,screen_height,max_x,max_y,min_x,min_y;
int actions[10];//Actions to send
int window_x, window_y;
} ev;
struct input_event *evnt;
void evdev_set_offset(x, y)
{
    ev.window_x=x, ev.window_y=y;
}
void evdev_touch_init()
{
    ev.id=0;
    ev.finger=ev.pfinger=-1;
    ev.min_x=ev.min_y=0;
    char i;
    for(i=0; i<10;i++)ev.actions[i]=1;
    char *path=get_config("evdev_mt_path");
    if(!path)path="/dev/event/input2";
    char *tmpptr=get_config("screen_width");
    if(!tmpptr||(ev.screen_width=atoi(tmpptr))<1);
    tmpptr=get_config("screen_height");
    if(!tmpptr||(ev.screen_height=atoi(tmpptr))<1);
    tmpptr=get_config("evdev_mt_min_x");
    if(tmpptr)ev.min_x=atoi(tmpptr);
    tmpptr=get_config("evdev_mt_min_y");
    if(tmpptr)ev.min_y=atoi(tmpptr);
    ev.max_x=ev.screen_width,ev.max_y=ev.screen_height;
    tmpptr=get_config("evdev_mt_max_x");
    if(tmpptr)ev.max_x=atoi(tmpptr);
    tmpptr=get_config("evdev_mt_max_y");
    if(tmpptr)ev.max_y=atoi(tmpptr);
    ev.fd=open(path,O_RDONLY|O_NONBLOCK);
    evnt=malloc(16);
}

int evdev_touch_update(struct SupportModule *module)
{
    int c=read(ev.fd,evnt,16);
    ev.pid=ev.id;
    if(c==16)ev.id++;
    while(c==16)
    {
        switch(evnt->code){
            case ABS_MT_POSITION_X:
                ev.x[ev.finger]=evnt->value;
                break;
            case ABS_MT_POSITION_Y:
                ev.y[ev.finger]=evnt->value;
                break;
            case ABS_MT_TRACKING_ID:
                ev.finger=evnt->value;
                break;
            case 48:
                if(evnt->value==0)ev.finger=-1;
                break;
            case 0:
                for(ev.state=0;ev.state<=ev.finger;ev.state++) {
                    if(ev.actions[ev.state]!=1)
                        ev.actions[ev.state]=ACTION_MOVE;
                    else
                        ev.actions[ev.state]=0;
                }
                for(ev.state=0;ev.state<=ev.pfinger-ev.finger;ev.state++)
                    ev.actions[ev.finger+ev.state+1]=ACTION_UP;
                ev.pfinger=ev.finger;
                break;
        }
        c=read(ev.fd,evnt,16);
    }
    int i;
    for(i=0;i<=ev.finger+ev.state&&ev.id>ev.pid;i++)
    {
        module->input(module,ev.actions[i],ev.min_x+ev.x[i]*ev.screen_width/ev.max_x-ev.window_x,ev.min_y+ev.y[i]*ev.screen_height/ev.max_y-ev.window_y,i);
        if(ev.actions[i]==ACTION_UP&&ev.state)ev.state--;
    }
    ev.state=0;
    return 0;
}
