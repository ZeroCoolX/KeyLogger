# KeyLogger 

DO NOT use this code for illegal or immoral purpose and keep it open source!
I do not give my permission or consent for any parties to use this for illegal or immoral reasons.

This is strictly for personal educational purposes and personal pen testing.

The Keylogger is able to capture all the keystrokes using system hooks. In addition to this, it is also able to put them into a file, encrypt the file and send the file via mail based on a timer.

This is strictly for educational and self learning purposes courtesy of following: https://www.udemy.com/course/how-to-create-an-advanced-keylogger-from-scratch-for-windows/learn/lecture/5803580#overview


## Setup instructions
* To compile it you need to enable C++11 standards and -mwindows flag
* In KeyHook.h modify the variable sendIntervalInMinutes to be however many minutes you want in between each email.
* Change the SMTP credentials wtihin SendMail.h to wherever you want to recieve the emails
  * X_EM_TO
  * X_EM_FROM
  * X_EM_PASS
 * In addition - ensure that you enable "Allow Less Secure Apps" on gmail for the accoutn
 * https://devanswers.co/allow-less-secure-apps-access-gmail-account/
 
 ## Usage instructions
 * The build target is named "Microsoft.cleanup.exe" by default.
   * On runtime you will have no indication of a processes running (window popup/flash) but you can find it in the TaskManager
 * It creates a folder on initial startup, in where it stores the log files that get emailed as well as where the PS script gets generated
   * %AppData%\Roaming\Microsoft\CLR
 * For debug purposes an AppLog.txt is written to the executing directory on startup
   * This is useful because the keylogger doesn't crash, it swallows exceptions and continues trying - otherwise too much attention and maintenence would be needed
     * For example Gmail is denying your credentials


# DecryptB64

Decrypts log files emailed by the Keylogger.

## Setup instructions
* To compile it you need to enable C++11 standard and DISABLE the -mwindows flag

 ## Usage instructions
 * Run the .exe providing 2 params, input file to decrypt and output file to write to
 
~\DecryptB64\bin\Debug>DecryptB64.exe "16.06.2019 09_33_53.log" "DecryptTest\decrypt.txt"
Decrypting file 16.06.2019 09_33_53.log
File 16.06.2019 09_33_53.log successfully decrypted to DecryptTest\decrypt.txt
