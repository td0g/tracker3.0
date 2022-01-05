Created by Tyler Gerritsen
vtgerritsen@gmail.com

Stand-Alone Telescope Driver for DeE (Deus Ex) Mount

1.0
	Initial Release
	
1.1	2017-09-10
	Code Clean-up
	Handle's non-resetting connection (allows mount to maintain position when switching applications)

2.0
	Code Re-Written in C#

2.01
	Added Debug, rising / falling options in setup dialog
	
3.0
	Changed to Local Server format
	Added Archangel Focuser support
	
3.01
	Modified code to reduce UART traffic
	Added SlewToTarget support

3.02	2017-09-23
	Fixed RA and DEC sync display bug
	
3.03	2017-10-01
	Added new Serial function: SendMessageValue recieves a byte array, more efficient than string
	
3.10	2018-12-14
	Update to communication protocol