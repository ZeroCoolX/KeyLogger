#include <iostream>
#include <windows.h>

int main(){
    // Intercept any window and thread messages
    // Translate it
    // Resend it so that GetMessage() will grab it again
    // This drops the UI Thread in an infinite loop so no window ever appears
    MSG Msg;
    while(GetMessage(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return 0;
}
