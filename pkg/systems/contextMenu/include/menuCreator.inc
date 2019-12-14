function addMenuOption( byref menu, cliloc, color := 0, disabled := 0, callback := 0 )

	var menuOption := struct{ "cliloc" := cliloc };

	if( color )
		menuOption.+color := color;
	endif

	if( disabled )
		menuOption.+disabled := 1;
	endif

	menu.menuOptions.append(menuOption);
	menu.callbacks.append(callback);

endfunction