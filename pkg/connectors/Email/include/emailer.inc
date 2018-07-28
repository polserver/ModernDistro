/* $Id: email.inc
 *
 * Purpose
 * Provide a single function to be able to quickly send email from within the server without external programs.
 * (Excluding an SMTP server)
 * 
 * Created by Dev GIB
 *
 */
use uo;
use os;


/*
 * SendEmail( ToAddress, Subject, Body, Modifiers)
 *
 * Purpose
 * Sends an email to address or addresses based on information given to the function.
 * Multiple emails can be defined for any given email field, its important to seperate with a semicolon
 *
 * Parameters
 * ToAddress:   The email address the email is being sent to.
 * Subject:     The subject line of the email body.
 * Body:        The text to be put into the body.
 * Modifiers:   A struct of modifiers that SMTP connections support.
 * -----------Modifier Options------------------------
 *     FromEmail:      To define an email address that the email is from otherwise default from the settings file will be used.
 *     ToContactName:  To define a name which will be displayed next to the email in the recipients mail client.
 *     CC:             To define carbon copy emails to be sent, carbon copy email addresses can be seen by all recipients.
 *     BCC:            To define blind carbon copy emails to be sent, the same as above only other receipients can't see who was BCC'ed.
 *     ReplyTo:        To define a different email address to be replied to. This is different to FromEmail, when a client replies to an email sent this email address would be used in place of FromEmail.
 *
 * Return value
 * Success - Returns value 1.
 * Failure - Returns error message.
 *
 */
function SendEmail( ToEmail, Subject, Body, Modifiers:=0)

	var settings_cfg := ReadConfigFile(":emailer:settings");
	if( settings_cfg.errortext )
		SysLog("Error::SendEmail() - Unable to open [:emailer:settings.cfg] ->"+settings_cfg.errortext);
		return error{"errortext" := "Unable to open [:emailer:settings.cfg]"};
	endif
	
	var cfg_elem := FindConfigElem( settings_cfg, "Settings" );
	
	var smtpHost := GetConfigString( cfg_elem, "host" );
	var LoggingLevel := cInt(GetConfigString( cfg_elem, "LoggingLevel" ));
	if( LoggingLevel == error )
		LogToFile("::log/emailer.log", "Error::SendEmail() - Logging level config not found or incorrect.", LOG_DATETIME);
	endif
	if( smtpHost == error )
		if( LoggingLevel >= 1 )
			LogToFile("::log/emailer.log", "Error::SendEmail() - SMTP host config not found or incorrect.", LOG_DATETIME);
		endif
		return error{"errortext" := "SMTP host config not found or incorrect."};
	endif
	var smtpPort := GetConfigString( cfg_elem, "port" );
	if( smtpPort == error )
		if( LoggingLevel >= 1 )
			LogToFile("::log/emailer.log", "Error::SendEmail() - SMTP port config not found or incorrect.", LOG_DATETIME);
		endif
		return error{"errortext" := "SMTP port config not found or incorrect."};
	endif
	
	if( !Modifiers )
		Modifiers := struct{};
		Modifiers.FromEmail := ""; 
		Modifiers.ToContactName := "";
		Modifiers.CC := "";
		Modifiers.BCC := "";
		Modifiers.ReplyTo := "";
	endif
	
	if( !Modifiers.FromEmail )
		Modifiers.FromEmail := GetConfigString( cfg_elem, "FromEmail" );//Sets the FromEmail to the default FromEmail in the config file.
	endif
	
	
	if( ValidateEmail( ToEmail ) != 1 )
		if( LoggingLevel >= 1 )
			LogToFile("::log/emailer.log", "Error::SendEmail() - ToEmail is not valid: "+ToEmail, LOG_DATETIME);
		endif
	   return error{"errortext":="ToEmail is not valid."};
	endif
	if( ValidateEmail( Modifiers.FromEmail ) != 1 )
		if( LoggingLevel >= 1 )
			LogToFile("::log/emailer.log", "Error::SendEmail() - Modifiers.FromEmail is not valid: "+Modifiers.FromEmail, LOG_DATETIME);
		endif
	   return error{"errortext":="FromAddress is not valid."};
	endif
	
	var EmailCommand := BuildEmailStruct( ToEmail, Subject, Body, Modifiers );
	if( LoggingLevel > 0 ) EmailCommand.LL := LoggingLevel; endif
	var connection := OpenConnection( smtpHost, cint(smtpPort), ":emailer:sendEmail", EmailCommand, 1 );
	if( connection == error )
		return connection;
	endif
	return 1;
	  
endfunction

/*
 * ValidateEmail( Address )
 *
 * Purpose
 * Checks a string to make sure it contains an @ symbol and a full stop.
 *
 * Parameters
 * Address:   The email address that is to be checked.
 *
 * Return value
 * Success - Returns value 1.
 * Failure - Returns value 0.
 *
 */

function ValidateEmail( Address )

  var HasAtSymbol := 0;
  var HasFullStop := 0;
  var i;
  
  for ( i:= 1; i< address.length(); i:=i+1)
    if( HasAtSymbol == 1 && HasFullStop == 1 )
    	return 1;
    endif
  	if ( address[i] == "@" )
  		HasAtSymbol := 1;
  	elseif( address[i] == "." );
  	  HasFullStop := 1;
  	endif
  endfor
  
  return 0;
   
endfunction



/*
 * BuildEmailStruct( ToEmail, Subject, Body, Modifiers )
 *
 * Purpose
 * Takes all the information supplied for building an email and then packs it all into an individual struct.
 * This struct is then used by the sendEmail script to construct a message to the SMTP server.
 *
 * Parameters
 * ToEmail:     The email address the email is being sent to.
 * Subject:     The subject line of the email body.
 * Body:        The text to be put into the body.
 * Modifiers:   A struct of modifiers that SMTP connections support.
 * -----------Modifier Options------------------------
 *     FromEmail:      To define an email address that the email is from otherwise default from the settings file will be used.
 *     ToContactName:  To define a name which will be displayed next to the email in the recipients mail client.
 *     CC:             To define carbon copy emails to be sent, carbon copy email addresses can be seen by all recipients.
 *     BCC:            To define blind carbon copy emails to be sent, the same as above only other receipients can't see who was BCC'ed.
 *     ReplyTo:        To define a different email address to be replied to. This is different to FromEmail, when a client replies to an email sent this email address would be used in place of FromEmail.
 *
 * Return value
 * Struct
 *
 */

function BuildEmailStruct( ToEmail, Subject, Body, Modifiers )
	
	var TempStruct := struct{};
	
	TempStruct.ToEmail := ToEmail;
	TempStruct.Subject := Subject;
	TempStruct.Body := Body;
	TempStruct.FromEmail := Modifiers.FromEmail;
	if( Modifiers.ToContactName ) TempStruct.ToContactName := Modifiers.ToContactName; endif
	if( Modifiers.CC ) TempStruct.CC := Modifiers.CC; endif
	if( Modifiers.BCC ) TempStruct.BCC := Modifiers.BCC;endif
	if( Modifiers.ReplyTo ) TempStruct.ReplyTo := Modifiers.ReplyTo; endif
	
	return TempStruct;
	
endfunction