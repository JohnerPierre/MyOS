// Func for wait a short time to understand the display
void wait(){
	int i = 0;
	while(i < 600000000){
		i++;
	}
}


int test1 (){
	char data[5];
	initDisplay();
	displayStringCursor(" Initialization");
	
	wait();
	
	displayStringCursor(" Clean Screen");
	
	wait();
	
	cleanDisplay();
	
	wait();
	
	setColorText(5);
	setBackgroundText(15);
	displayStringCursor(" Change color and background");
	
	wait();
	
	displayStringCursor(" Display Color");
	itos(getColorText(),data, 10);
	displayStringCursor(data);
	itos(getBackgroundText(),data, 10);
	displayStringCursor(" and ");
	displayStringCursor(data);
	
	wait();
	
	displayStringCursor(" Display multiple Char");
	
	wait();
	
	displayStringCursor(" Display cursor position ");
	itos(getCursor(),data, 10);
	displayStringCursor(data);
	
	wait();
	
	setCursor(260);
	displayStringCursor(" Cursor movement");
	
	displayStringCursor(" Display cursor position ");
	itos(getCursor(),data, 10);
	displayStringCursor(data);
	
	wait();
	
	displayStringCursor(" Display cursor position ");
	itos(getCursor(),data, 10);
	displayStringCursor(data);
	
	wait();
	
	cleanDisplay();
	setCursorPos(0,0);
	displayStringCursor(" print : ");
	print(" Display 42 in String : %s in Decimal : %d in Hex : %x Display S in char : %c", "42",42,42,'S');

	wait();
	
	while(1){
	    print("Touche : %c",getc());
	}
	
	
    return 0;
}