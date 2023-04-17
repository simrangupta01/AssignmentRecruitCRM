Action()
{
	web_set_sockets_option("SSL_VERSION", "AUTO");
	
	web_cache_cleanup();

	web_cleanup_cookies();

		web_reg_find("SaveCount=Launch_success",
		"Text=Welcome to the Simple Travel Agency!",
		LAST);
	
	
	lr_start_transaction("T01_Launch");
	
	web_url("{p_URL}", 
		"URL=https://{p_URL}/", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=", 
		"Snapshot=t8.inf", 
		"Mode=HTML", 
		LAST);

	
	if((atoi(lr_eval_string("{Launch_success_count}")) > 0))
	{
	lr_end_transaction("T01_Launch",LR_AUTO);
	}
	else
	{
		lr_end_transaction("T01_Launch",LR_AUTO);
		lr_output_message("Page  has not been launched");
	}

	lr_think_time(10);
	
	web_reg_save_param_ex(
		"ParamName=c_FlightReserve",
		"LB=<h3>",
		"RB=</h3>",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);

	lr_start_transaction("T02_FindFlights");
	
	web_submit_data("reserve.php", 
		"Action=https://{p_URL}/reserve.php", 
		"Method=POST", 
		"RecContentType=text/html", 
		"Referer=https://{p_URL}/", 
		"Snapshot=t12.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=fromPort", "Value={p_fromPort}", ENDITEM, 
		"Name=toPort", "Value={p_toPort}", ENDITEM, 
		LAST);

	if((atoi(lr_eval_string("{c_FlightReserve_count}")) > 0))
	{
	lr_end_transaction("T02_FindFlights",LR_AUTO);
	
	}
	
	else
	{
		lr_end_transaction("T02_FindFlights",LR_AUTO);
		lr_output_message("Flights not chosen");
	}

	lr_think_time(10);
	
	//<p>Flight Number: UA954</p>:
	web_reg_save_param_ex(
		"ParamName=c_FlightNumber",
		"LB=<p>Flight Number: ",
		"RB=</p>",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);
	
	lr_start_transaction("T03_ChooseFlight");
	
	web_submit_form("purchase.php", 
		"Ordinal=2", 
		"Snapshot=t13.inf", 
		ITEMDATA, 
		LAST);

	if((atoi(lr_eval_string("{c_FlightNumber_count}"))) > 0)
	   {

			lr_end_transaction("T03_ChooseFlight",LR_AUTO);
	   }	
	else
		{
			lr_end_transaction("T03_ChooseFlight",LR_AUTO);
		}
		   
	lr_think_time(10);
	
	web_reg_find("SaveCount=Booking_count",
		"Text=Thank you for your purchase today!",
		LAST);

	
	lr_start_transaction("T04_Confirmation");

	
	web_submit_data("confirmation.php", 
		"Action=https://{p_URL}/confirmation.php", 
		"Method=POST", 
		"RecContentType=text/html", 
		"Referer=https://{p_URL}/purchase.php", 
		"Snapshot=t15.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=_token", "Value=", ENDITEM, 
		"Name=inputName", "Value={p_name}", ENDITEM, 
		"Name=address", "Value={p_address}", ENDITEM, 
		"Name=city", "Value={p_city}", ENDITEM, 
		"Name=state", "Value={p_State}", ENDITEM, 
		"Name=zipCode", "Value={p_zip}", ENDITEM, 
		"Name=cardType", "Value={p_cardType}", ENDITEM, 
		"Name=creditCardNumber", "Value={p_creditCardNum}", ENDITEM, 
		"Name=creditCardMonth", "Value=12", ENDITEM, 
		"Name=creditCardYear", "Value=2021", ENDITEM, 
		"Name=nameOnCard", "Value={p_nameOnCard}", ENDITEM, 
		LAST);
	
	if((atoi(lr_eval_string("{Booking_count}")) > 0))
		
	{
	lr_end_transaction("T04_Confirmation",LR_PASS);
	
	}

	else
	{
		lr_end_transaction("T04_Confirmation",LR_FAIL);
	
	}
	

	return 0;
}