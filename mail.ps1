Param(
	[String]$Att,
	[String]$Subj,
	[String]$Body
	)
	
Function Send-Email{
	Param(
			[Parameter(`
			Mandatory=$true)]
		[String]$To,
			[Parameter(`
			Mandatory=$true)]
		[String]$From,
			[Parameter(`
			Mandatory=$true)]
		[String]$Password,
			[Parameter(`
			Mandatory=$true)],
		[String]$Subject,
			[Parameter(`
			Mandatory=$true)],
		[String]$Body,
			[Parameter(`
			Mandatory=$true)],
		[String]$Attachment,
			[Parameter(`
			Mandatory=$true)]
		)
		
	try{
		$msg = New-Object System.Net.Mail.MailMessage($From, $To, $Subject, $Body)
		$srvr = "smtp.gmail.com"
		if($Attachment -ne $null){
			try{
				$attachments = $Attachment -split ("\:\:");
				
				ForEach($val in $attachments){
					$attch = New-Object System.Net.Mail.Attachment($val)
					$msg.Attachments.Add($attch)
				}
			}catch{
				exit 2;
			}
			
			$SMTPClient = New-Object System.Net.Mail.SmtpClient($srvr, 587)
			$SMTPClient.EnableSsl = $true
			$SMTPClient.Credentials = New-Object System.Net.NetworkCredential($From.Split("@")[0], $Password)
			$SmtpClient.Send($msg)
			Remove-Variable -Name SmtpClient
			Remove-Variable -Name Password
			exit 7;
		}
	} catch{
		exit 3;
	}
}

try{
	Send-Email
		-Attachment $Att
		-To "Address of the recipient"
		-Body $Body
		-Subject $Subj
		-Password "foobar"
		-From "Address of the sender"
}catch{
	exit 4;
}




