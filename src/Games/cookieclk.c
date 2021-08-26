

#include "../common.h"
#include "../all_drivers.h"

extern bool lmouseDown;
extern int16_t mouseX;
extern int16_t mouseY;

char cookieNormal[16][52] = {                                           
"                 .........                        \n",
"            ...*/,,,*,,,,.,..*..,,,               \n",
"          ../*,,.*,,,,.%/**.,*,,,,.*#*.           \n",
"       ..,*/,*,*&#(.,/,.(...,**,,..*,..** .       \n",
"      .,/**/#(.*/,.//#%**,/#, *,%#(*.,,*# ..      \n",
"     .,*((&.,*,.##(*,(,./(**/,*,,.,,*.**.*, .     \n",
"    .**//,**,***/..,*#((,,..***/*,,#,.,,*/*. .    \n",
"   .,*(*,..%#,*#(/..#%/*/&..(%#*,,((*,*../.,,.    \n",
"   .,//*,*...,*/(*..*,,,//,**.**,/,%*/,.(*((,,.   \n",
"  ...*(&%,,,..,,,.,/*&*,*(#,*/(*,..&,.,*,*#.*..   \n",
"   ..*#/*%#(,*%**(//,,,,%(&,.(%/(,,,.,((..(,.     \n",
"   ...*(((#/**.*.,/##*,&,..,/,/.,*.,,,((,...      \n",
"     ..,*/(/&##//,.,.,,*,/#.(*..*,%.,,,,* .       \n",
"        ..,/%*,,(,/%(,.,.*,,,.,*%#****.,,         \n",
"          .../(*/(#%(,*#(*/*/*,,*../*..           \n",
"              ..,//*****//**,##*,*,               \n",
"                    ........                      \n"};     

char cookieClicked[16][52] = {
"                                                  \n",
"                                                  \n",
"                .,,,/,%(,.//,..,,                 \n",
"            ../(,.,,.,.((,,.,*,,,,,               \n",
"          .*/(***#%,,(./..,..##,*,,.,             \n",
"         .//#%//*,..(##..#%#,,.,,...              \n",
"       .,**/,/..,%#,,/./,//,*,,**..*.*.           \n",
"       .*&*,.#&,(/,.%%((/.,##,,(%.*..**/          \n",
"      ../#*.**../*/.,,,.***/,*/.#(../**(,         \n",
"       .*(%&,*./(,,*./,%(%**#,,..,.,,.,,          \n",
"       .,/&%(%,...(**,*.,,,*,,,,.,%#,.*.          \n",
"        ..*///%#/,,*,.,*/#/*.*.#..,,,..           \n",
"           ..*#*///(/*,,,,,,,&%/,.*,,             \n",
"              .*(*(/*,**,*,**,*/*                 \n",
"                 ....*((**....                    \n",
"                                                  \n"};

void CookieClickerGame(){

    // TO DO //

    // instead of using print_at,
    // use a double buffer
    // to prevent tearing
    // and in general easier
    // programming

    // actual stuff into the thing bruh

    terminal_initialize();
    int clicks, _timer, oldclicks, cps;
    bool alreadydown = false;
    uint8_t key = 0;
    _timer = startTimer();
    clicks = 0;
    oldclicks = 0;
    cps = 0;

    
    while(true){
        terminal_setcolor(VGA_COLOR_BROWN);
        if(!alreadydown){
            for(int i = 0; i<16; i++){
                print_at(cookieNormal[i], 0,i);
            }
        } else {
            for(int i = 0; i<16; i++){
                print_at(cookieClicked[i], 0,i);
            }
        }
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        print_at("Clicks: ", 8*2, 18);
        print_at("          ", 8*3, 18); // clear
        print_at(itoa(clicks, 10), 8*3, 18);
        print_at("CPS   : ", 8*2, 19);
        print_at("          ", 8*3, 19); // clear
        print_at(itoa(cps, 10), 8*3, 19);
        print_at("don't take this as a serious game. "
                "take it as a CPS counter", 0, 22);
        print_at("press ESC to exit.", 0, 24);
        
        if(getTimerMs(_timer) >= 1000){
            stopTimer(_timer);
            cps = oldclicks/1;
            oldclicks = 0;
            _timer = startTimer();
        }
        key = old_keyboard_read_key();
        if(key == 0x1){
            break;
        }
        if(lmouseDown){
            if(mouseX < 47 && mouseY < 16 && !alreadydown){
                clicks++;
                oldclicks++;
                alreadydown = true;
            }
        } else {
            alreadydown = false;
        }

    }
    terminal_initialize();
}



