#include <fcntl.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

char InputBuffer[4096*70+1];
char OutputBuffer[4096*10];
char ReadString[1024];

int CharCounts[256];
int BeforeCounts[1024];
int LessCounts[256];

int main(int argc, char **argv)
{
   char ch;
   int t;
   char *pBeg;
   int  *pI,*pO,*pE;
   char *InputBufferPtr, *InputBufferEnd;
   char *OutputBufferPtr, *OutputBufferEnd;

   int  Index = 0;
   int  StringLength = 0;
   int  ReadingNumber = 1;

   int  fh = open(argv[1],O_RDONLY);

   InputBufferPtr = InputBufferEnd = InputBuffer+sizeof(InputBuffer);
   OutputBufferPtr = OutputBuffer;
   OutputBufferEnd = OutputBuffer + sizeof(OutputBuffer)-1;
  
   while(1)
   {
      /* When needed read additional data */
      if (InputBufferPtr >= InputBufferEnd)
      {
         InputBufferEnd = InputBuffer + read(fh,InputBuffer,sizeof(InputBuffer));

         /* No more data, we're done */
         if (InputBufferEnd == InputBuffer)
            break;
         InputBufferPtr = InputBuffer;
      }

      ch = *InputBufferPtr++;

      /* See if we're done with this line */
      if (ch == '\n')
      {
         /* Flush the output buffer if we're near the end */         
         if (OutputBufferPtr + StringLength > OutputBufferEnd)
         {
            write(1,OutputBuffer,OutputBufferPtr - OutputBuffer);
            OutputBufferPtr = OutputBuffer;
         }

         /* Determine how many characters we have that are before each character, using their ASCII value */
         for (pI = CharCounts + 'A',pO = LessCounts + 'A',pE = CharCounts + 'z' + 1,t=0;
              pI < pE;
              pI ++,pO++)
         {
            *pO = t;
            t += *pI;
            *pI = 0;
         }                      

         /* Now decode the string starting at the end */
         pBeg = OutputBufferPtr;
         OutputBufferPtr+=StringLength-1;
         *OutputBufferPtr = ReadString[Index];
         for (ch = *OutputBufferPtr,OutputBufferPtr--;
              OutputBufferPtr >= pBeg;
              ch = *OutputBufferPtr,OutputBufferPtr--)
         {
            Index = LessCounts[ch] + BeforeCounts[Index];
            *OutputBufferPtr = ReadString[Index];
         }

         /* Move past the string and add a line feed */
         OutputBufferPtr+=StringLength+1;
         *OutputBufferPtr++='\n';

         /* Reset the variables */
         memset(BeforeCounts,0,StringLength * sizeof(BeforeCounts[0]));
         ReadingNumber = 1;
         StringLength = Index = 0;
      }      
      else if (ch == ' ')
         ReadingNumber = 0;
      else if (ReadingNumber)
         Index = Index*10 + ch - '0';
      else
      {
         BeforeCounts[StringLength] = CharCounts[ch];
         CharCounts[ch] ++;
         ReadString[StringLength++] = ch;
      }
   }
   
   /* Print our output buffer */
   if (OutputBufferPtr != OutputBuffer)
      write(1,OutputBuffer,OutputBufferPtr - OutputBuffer);

   _exit(0);
   return 0;
}