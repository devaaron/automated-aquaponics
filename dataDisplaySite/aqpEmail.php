<?php
			//$emailTo = 'am-snowden@wiu.edu';
			$emailTo = 'am-snowden@wiu.edu';
			$body = "This is the information ";
			$subject = "Triathlon Form";
			$name = "Aquaponics System Admin";
			$email = "am-snowden@wiu.edu";
			$headers = 'From: '.$name.' <'.$email.'>' . "\r\n" . 'Reply-To: ' . $email;	
			//If not from Blocked IP, then send email
			mail($emailTo, $subject, $body, $headers);
?>
     <html><body>
     <form action="what_ever.ext" method="post" name="MyForm">
       <input type="hidden" name="hiddenfield1" value="<? echo $some_important_value; ?>">

     </form>
     
     <script language="javascript" type="text/javascript">
    document.MyForm.submit();
     </script>
     <noscript><input type="submit" value="verify submit"></noscript>

     </body></html>