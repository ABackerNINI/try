@ECHO OFF

for /l %%i in (100,-1,1) do (

	IF EXIST %%i.TXT ( 
		ECHO.
		ECHO.
		ECHO TEST%%i:
		ECHO.
		TYPE %%i.TXT
		ECHO.
		ECHO.
		TYPE %%i.TXT | "template for ACM.exe" >RESULT.TXT 

		TYPE RESULT.TXT

		PAUSE>NUL
	)
)

:END