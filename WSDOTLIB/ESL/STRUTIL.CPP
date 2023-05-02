///////////////////////////////////////////////////////////////////////
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This library was developed as part of the Alternate Route Project
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the Alternate Route Library Open Source License as published by 
// the Washington State Department of Transportation, Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but is distributed 
// AS IS, WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE. See the Alternate Route Library Open Source 
// License for more details.
//
// You should have received a copy of the Alternate Route Library Open Source License 
// along with this program; if not, write to the Washington State Department of 
// Transportation, Bridge and Structures Office, 4500 3rd Ave SE - P.O. Box  47340, 
// Olympia, WA 98503, USA or e-mail Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#include <esl\strutil.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

/****************************************************************/
/****************************************************************/
/*                                                              */
/*     CHARACTER/NUMBER GET AND PUT INTO STRING                 */
/*                                                              */
/****************************************************************/
/****************************************************************/

/*************************************************************
   GET INTEGER
*************************************************************/
int geti( char *line,   /* String buffer */
          int  col,     /* Line Comumn for start of field */
          int  width)   /* Width of field */

{
   char     c;
   int      i;
   int    val;

   i = col + width -1;
   c = line[i];
   line[i] = 0;
   if ( sscanf(&line[col-1],"%ld",&val) != 1 )
      val = 0;
   line[i] = c;
   return( (int)val );
}

/*************************************************************
   GET REAL ( yeaa!!! )
*************************************************************/
double getf( char *line, /* String Buffer */
             int  col,   /* Line Comumn for start of field */
             int  width) /* Width of field */

{
   char     c;
   int      i;
   double  val;

   i = col + width -1;
   c = line[i];
   line[i] = 0;
   if ( sscanf(&line[col-1],"%lf",&val) != 1 )
      val = 0.0;
   line[i] = c;
   return( val );
}

/*************************************************************
   GET STRING FROM "LINE"
*************************************************************/
void  getstg( char   *line,   /* String Buffer */
                      int    col,   /*  Line Comumn for start of field */
                      int  width,   /* Width of field */
                      char    *stg,    /* Address of destination */
                      bool leftJustify) // left justifies stg in buffer
{
   unsigned int   i;
   char* cptr;

   strncpy(stg,&line[col-1],width);
   stg[width] = 0;
   if (leftJustify)
      {
      for(cptr=stg, i=0;*cptr == ' ' && i < strlen(stg);cptr++,i++);
      strcpy(stg,cptr);
      }
   for (i=strlen(stg)-1; (i>0) && (stg[i] <= ' '); i--);
   stg[i+1] = 0;
}

// Get a letter (character)
char getltr(char* line,int col)
{
   col--;
   return line[col];
}

/*************************************************************
   Put Integer into LINE char Buffer
*************************************************************/
void  puti( char *line,  /* Dest. String Buffer */
            int  num,    /* Value to insert into string */
            int  start,  /* Start Column. First Column = 1 */
            int  width)  /* Width of Field */
{
   char  fmt[20];
   char  stg[20];
   int   i;

   start--;
//   sprintf(fmt,"%c%ldd",'%',width);
   sprintf(fmt,"%c%dd",'%',width);
   sprintf(stg,fmt,num);
   for (i=0; stg[i] != 0; i++ )
      line[i+start] = stg[i];
}

/*************************************************************
   Put Float into LINE char Buffer
*************************************************************/
void  putf( char   *line,   /* Dest. String Buffer */
            double num,     /* Value to insert into string */
            int    start,   /* Start Column. First Column = 1 */
            int    width,   /* Width of Field */
            int    dec)     /* Number of decimal points */
{
   char  stg[60];
   int   i;

   start--;

   sprintf(stg,"%*.*lf",(int)width,(int)dec,num);

   while ( ((int)strlen(stg) > width) && (dec >= 0) ) {
      sprintf(stg,"%*.*lE",(int)width,(int)dec,num);
      dec--;
   }

   if ( (int)strlen(stg) > width ) {
      if ( num >= 0.0L )
         strcpy(stg,"*INF*");
      else
         strcpy(stg,"*NEG*");
   }
   for (i=0; stg[i] != 0; i++ )
      line[i+start] = stg[i];

}

void putchr(char *line,char c,int col)
{
   col--;
   line[col] = c;
}

/*************************************************************
   Put Character string into LINE buffer
*************************************************************/
void  putstg( char *line,   /* Dest. string buffer */
              const char stg[],   /* Src.  string buffer */
              int  col,     /* Start Column. First Column = 1 */
              int  len)     /* Width of Field */
{
   unsigned short   i;

   for (i=0;(len>0) && (stg[i] != 0); len--,col++,i++ )
      line[col-1] = stg[i];
}

/***********************************************************************
   Initialize string with at most numChar characters c
************************************************************************/
void   initstg(char *line, char c, int numChar)
{
   int i;

   for (i=0; i < numChar; i++)
      line[i] = c;
   line[numChar-1] = 0; /* NULL terminate the string */
}

// Returns true if the string has nothing but white space

bool IsBlankStg(char* line,int numChar)
{
   unsigned short i;
   for (i=0; i < numChar; i++)
      {
      if (!isspace(line[i]) && line[i] != 0)
         return false;
      }

   return true;
}

int GetFeet(double dim,int dimType)
{
   if (dimType == DT_METER)
      {
      dim /= 0.0254; // convert to inch
      dimType = DT_INCH;
      }

   if (dimType == DT_INCH)
      dim = (dim + 0.5)/12.; // convert to feet;

   return (int)(dim);
}

double GetInch(double dim,int dimType)
{
   int feet = GetFeet(dim,dimType);

   if (dimType == DT_METER)
      {
      dim /= 0.0254; // convert to inch
      dimType = DT_INCH;
      }

   if (dimType == DT_INCH)
      dim /= 12.; // convert to feet;

   dim -= feet;  // remove feet
   dim *= 12.;   // convert to inch

   return dim;
}

