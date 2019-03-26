
/*
*
*	The Kernel
*
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "kernel.h"
#include "hal/hal.h"
#include "string.h"

// #include "drivers/TextEditor/simpleTextEditor.h"

#include "drivers/stdio/emb-stdio.h"			// Needed for printf
#include "drivers/sdcard/SDCard.h"


void kernel_init(void);
void input_output_init(void);
void sys_info( uint8_t* );
void sd_card_fs_demo();
void printf_sysinfo(void);
void DisplayDirectory(const char*);
void cat_file(const char*);
void backspace(char *);
void textEditor(void);
void read_file(void);
void input_file(void);

char bufferb[15][50]; // max 15 files , each one max 50 words
int posBufferb1 = 0;
int posBufferb2 = 0;  

/*
 *		Kernel's entry point
**/
void main(uint32_t r0, uint32_t r1, uint32_t atags){

  //Init
  kernel_init();
  input_output_init();

    //<<-- Uncomment this to show File System/SD Card demo
  sdInitCard (&printf_serial, &printf_serial, true); // init the sd card 
  //  sd_card_fs_demo(); // print Alice file in this case

  //Welcome Msg Video
  hal_io_video_puts( "\n\r\n\rWelcome to MiniOS Pi Zero\n\r", 3, VIDEO_COLOR_GREEN );
  hal_io_serial_puts( SerialA, "\n\r\n\rWelcome to MiniOS Pi Zero\n\r" );
  hal_io_video_puts( "\n\r$ ", 2, VIDEO_COLOR_GREEN );
  hal_io_serial_puts( SerialA, "\n\r$ " );

  uint8_t c;
	char buffer[100];
  char *filePath;	
 
  int posBuffer = 0;
 
  int posFile = 0;           


  bool printCheck = true;
	while (1){

    c = hal_io_serial_getc( SerialA );
    
    if(c !='\r' && c !=' ' && printCheck){
        
        printf_video( "%c", c );  //<<--- We also have printfs
        printf_serial( "%c", c );
        buffer[posBuffer] = c;
        posBuffer++;
    }

    if(c ==' '){
        printCheck = false;
        printf_video( "%c", c );  
        printf_serial( "%c", c );
    }

    if(c !=' ' && !printCheck){
        printf_video( "%c", c );  //<<--- We also have printfs
        printf_serial( "%c", c );

         filePath[posFile]= c;
          posFile++;  
    }

    // if (c == '\b'){
    //    if (strlen(buffer) > 0){
		// 		  backspace(buffer);
    //       // printf_video("go back"); 
    //       // printf_serial("go back");  
		// 		  hal_io_serial_puts(SerialA, "\b \b");
		// 		  printf_serial("%s", "\b \b");
    //       printf_video("%s", "\b \b");
		// 	}
    // }

          buffer[posBuffer] = '\0';
          

    if(c =='\r'){
        printCheck = true;
  
        if(strcmp(buffer, "sysinfo")==0){
          hal_io_video_puts( "\n\r$ ", 2, VIDEO_COLOR_GREEN );
          hal_io_serial_puts( SerialA, "\n\r$ " );
          printf_sysinfo();
          posBuffer = 0;
        }

        if(strcmp(buffer, "ls")==0){
          
          printf_serial("Directory (/): \n");
	        DisplayDirectory("\\*.*");
          posBuffer = 0;
          printf_video("\n\r");
        }

        if(strcmp(buffer,"cat")==0){
          // posBuffer = 0;
          char filePath2[100];
          int size  = strlen(filePath)- 1;
          memcpy(filePath2, filePath, size);  
          // printf_serial("\n");
          // printf_video("%d\n",strlen(filePath2));
          // printf_serial("%d\n", strlen(filePath2));
          cat_file(filePath2);
          posBuffer = 0;
          printf_video("\n\r");
        }

        if(strcmp(buffer, "td")==0){
            textEditor();  
        }

    }

  }

}

void textEditor(){
  
  uint8_t d;

    do{
          hal_io_clear_screen();
          printf_video("\r");
          printf_video("Welcome to text Editor:\n\r");
          printf_serial("\n");
          printf_serial("\rWelcome to text Editor:\n\r");

          printf_serial("\n\rRead files enter 1, new files enter 2, Exit enter 3 :\n\r");
          printf_video("\n\rRead files enter 1, new files enter 2, Exit enter 3:\n\r");
          
          d = hal_io_serial_getc( SerialA );
          printf_serial("%c\n\r", d);
          printf_video("%c\n\r", d);

        if(d=='1'){
          read_file();
          break;
        }

        if(d=='2'){
          input_file();
          break;
        }
    }while(d!='3');
  }


void input_file(){

  bool check =true;
  uint8_t x;
  posBufferb2 = 0;

   printf_serial("\n\rHit Enter to save File:\n\r");
  printf_video("\n\rHit Enter to save File:\n\r");
      while(check){ 
          if(x =='\r'){
          printf_video("\n\rFile Save\n\r");  //<<--- We also have printfs
          printf_serial("\n\rFile Save\n\r");
            for(int i =0;i<=posBufferb2;i++){
              printf_serial("%c",bufferb[posBufferb1][i]);
              printf_video("%c",bufferb[posBufferb1][i]);
            }
              posBufferb1++;
              printf_serial("%d\n",posBufferb1); 
              check=false;
          }else{
            x = hal_io_serial_getc( SerialA );
            printf_video( "%c", x );  //<<--- We also have printfs
            printf_serial( "%c", x );
            bufferb[posBufferb1][posBufferb2] = x;
            posBufferb2++;
          }
      }
}

void read_file(){
  uint8_t y;
  char* str = "You have ";
  char* str2 =" files, which one you want to read?\n\r";

  if(posBufferb1>0){
    printf_serial(str);
    printf_serial("%d", posBufferb1);
    printf_serial(str2);
    y = hal_io_serial_getc( SerialA );
    
    printf_video( "%c", y );
    printf_serial( "%c", y );

    if( y=='\r'){
      int yy = y;

      // for(int i = 0; i<= 10; i++){  // can't print the value
      //   printf_serial("%c",bufferb[yy][i]);
      //   printf_video("%c", bufferb[yy][i]);
      // }
    }
  }else{
    printf_serial("You have 0 files ");
  }
}


// void backspace(char *p){
// 	int c = 0;

// 	while (p[c] != '\0')
// 	{
// 		c++;
// 	}
// 	if (c >= 1)
// 	{
// 		p[c - 1] = '\0';
// 	}
// }

void printf_sysinfo(){
          printf_video("hello this is my OS v`7\n\r");
          printf_serial("hello this is my OS v`7\n\r");
}

void cat_file(const char *fileName){

  char bufferCat[500];
  printf_serial("\n");
  printf_serial("Opening File \n\r");
  // printf_serial("%s\n", fileName);
  // printf_video("%s\n", fileName);
  printf_serial("\n\r");
  printf_video("\n\r");

  // printf_video("%d\n",strlen(fileName));
  // printf_serial("%d\n",strlen(fileName));

  HANDLE fHandle = sdCreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  
  if (fHandle != 0) {
    uint32_t bytesRead;

    if ((sdReadFile(fHandle, &bufferCat[0], 500, &bytesRead, 0) == true))  {
        bufferCat[bytesRead-1] = '\0';  ///insert null char
        printf_serial("File Contents: %s", &bufferCat[0]);
        printf_video("File Contents: %s", &bufferCat[0]);
    }
    else{
      printf_serial("Failed to read" );
    }

    // Close the file
    sdCloseHandle(fHandle);
  }
  
  printf_serial("\n");
  printf_video("\n");
}


/*
* Initializes the kernel
*/
void kernel_init(void){

  hal_io_init();
  //console_init();
  //system_calls_init();
  //scheduler_init();
  //faults_init();

}

/*
* Initializes All IO
*/
void input_output_init(void){
  uint32_t video_init_res = HAL_FAILED;

#ifdef VIDEO_PRESENT
  video_init_res = hal_io_video_init();
#endif

#ifdef SERIAL_PRESENT
  hal_io_serial_init();
#endif
//NOTE: PAST THIS POINT WE CAN USE printf
//      (printf needs both serial and video inits to be called first)

  if ( video_init_res == HAL_SUCCESS )
    sys_info( "Video Initialized\n\r" );
  else
    sys_info( "Video Init Failed [x]\n\r" );

    sys_info( "Serial Initialized\n\r" );
}

void sys_info( uint8_t* msg ){
  printf_video( msg );
  printf_serial( msg );
}

/////////////////////////////////////////////////////////////////
////////////////    D E M O    C O D E    ///////////////////////
/////////////////////////////////////////////////////////////////

// char buffer[500];


void sd_card_fs_demo(){
  // printf_serial("\n\n");
  // sdInitCard (&printf_serial, &printf_serial, true);

  // /* Display root directory */
  // printf_serial("\n\nDirectory (/): \n");
  // DisplayDirectory("\\*.*");

  // printf_serial("\n");
  // printf_serial("Opening Alice.txt \n");

  // HANDLE fHandle = sdCreateFile("Alice.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  // if (fHandle != 0) {
  //   uint32_t bytesRead;

  //   if ((sdReadFile(fHandle, &buffer[0], 500, &bytesRead, 0) == true))  {
  //       buffer[bytesRead-1] = '\0';  ///insert null char
  //       printf_serial("File Contents: %s", &buffer[0]);
  //   }
  //   else{
  //     printf_serial("Failed to read" );
  //   }

  //   // Close the file
  //   sdCloseHandle(fHandle);

  // }

}

void DisplayDirectory(const char* dirName) {
	HANDLE fh;
	FIND_DATA find;
	char* month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	fh = sdFindFirstFile(dirName, &find);							// Find first file
	do {
		if (find.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			printf_serial("%s <DIR>\n", find.cFileName);
      printf_video("%s <DIR>\n", find.cFileName);
    }
		else {
      printf_serial("%c%c%c%c%c%c%c%c.%c%c%c Size: %9lu bytes, %2d/%s/%4d, LFN: %s\n",
			find.cAlternateFileName[0], find.cAlternateFileName[1],
			find.cAlternateFileName[2], find.cAlternateFileName[3],
			find.cAlternateFileName[4], find.cAlternateFileName[5],
			find.cAlternateFileName[6], find.cAlternateFileName[7],
			find.cAlternateFileName[8], find.cAlternateFileName[9],
			find.cAlternateFileName[10],
			(unsigned long)find.nFileSizeLow,
			find.CreateDT.tm_mday, month[find.CreateDT.tm_mon],
			find.CreateDT.tm_year + 1900,
			find.cFileName);
    }
      
      										// Display each entry

    printf_video("%c%c%c%c%c%c%c%c.%c%c%c Size: %9lu bytes, %2d/%s/%4d, LFN: %s\n",
			find.cAlternateFileName[0], find.cAlternateFileName[1],
			find.cAlternateFileName[2], find.cAlternateFileName[3],
			find.cAlternateFileName[4], find.cAlternateFileName[5],
			find.cAlternateFileName[6], find.cAlternateFileName[7],
			find.cAlternateFileName[8], find.cAlternateFileName[9],
			find.cAlternateFileName[10],
			(unsigned long)find.nFileSizeLow,
			find.CreateDT.tm_mday, month[find.CreateDT.tm_mon],
			find.CreateDT.tm_year + 1900,
			find.cFileName);
	} while (sdFindNextFile(fh, &find) != 0);						// Loop finding next file
	sdFindClose(fh);												// Close the serach handle
}
