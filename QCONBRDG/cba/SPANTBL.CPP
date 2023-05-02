///////////////////////////////////////////////////////////////////////
// QConBridge - HL93 Live Load Analysis
// Copyright (C) 1999  Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// 4500 3rd AVE SE - P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Revision Log
// ============
// May 1997 - Created, Richard Brice, PE
// Oct 1999 - Released as Open Source

#include <qconbrdg\spantbl.h>
#include <qconbrdg\common.rh>
#include <owltools\numvalid.h>
#include <owltools\unitedit.h>

//
// SpanTable
//
DEFINE_RESPONSE_TABLE1(SpanTable,TGrid)
    EV_COMMAND(CM_INSERT, CmInsert),
    EV_COMMAND(CM_DELETE, CmDelete),
    EV_COMMAND_ENABLE(CM_DELETE,CeDelete),
    EV_COMMAND(CM_DUPLICATE, CmDuplicate),
    EV_COMMAND(CM_CLEARALL,CmClearAll),
END_RESPONSE_TABLE;

SpanTable::SpanTable(TWindow *parent,SystemOfUnits units,int resId,TModule *module) :
      TGrid (parent,resId, NextLine,TabNextColumn,ReverseColor,1,
             gAllBorders | gHideScroll | gCenterTitle | gHorzShrink | gVertShrink,
             module),
      units(units)
{
    // use parent and not "this" for the parent of the grid controls!!!
    if (units == unitsSI)
       {
       loc = new TGLengthEdit(parent,IDC_LOCATION,SIConstants::meter,11,3,IDC_LOCATION_TAG,"Location: m");
       Ax = new TGLength2Edit(parent,IDC_AX,SIConstants::millimeter2,11,3,IDC_AX_TAG,"Ax: mm^2");
       Iz = new TGLength4Edit(parent,IDC_IZ,SIConstants::millimeter4,11,3,IDC_IZ_TAG,"Iz: mm^4");
       ModE = new TGPressureEdit(parent,IDC_MODE,SIConstants::Pascal,11,3,IDC_MODE_TAG,"Mod E: Pa");
       unitWeight = NULL;
       density = new TGMaterialDensityEdit(parent,IDC_UNITWEIGHT,SIConstants::kilogram_per_meter3,11,3,IDC_UNITWEIGHT_TAG,"Density: kg/m^3");
       }
    else
       {
       loc = new TGLengthEdit(parent,IDC_LOCATION,USConstants::feet,11,3,IDC_LOCATION_TAG,"Location: feet");
       Ax = new TGLength2Edit(parent,IDC_AX,USConstants::inch2,11,3,IDC_AX_TAG,"Ax: inch^2");
       Iz = new TGLength4Edit(parent,IDC_IZ,USConstants::inch4,11,3,IDC_IZ_TAG,"Iz: inch^4");
       ModE = new TGPressureEdit(parent,IDC_MODE,USConstants::kip_per_inch2,11,3,IDC_MODE_TAG,"Mod E: ksi");
       density = NULL;
       unitWeight = new TGUnitWeightEdit(parent,IDC_UNITWEIGHT,USConstants::pound_per_feet3,11,3,IDC_UNITWEIGHT_TAG,"Unit Wgt: pcf");
       }

    s_gMoment = new TGDoubleEdit(parent,IDC_S_GMOMENT,11,3,0,999.99);
    s_gShear = new TGDoubleEdit(parent,IDC_S_GSHEAR,11,3,0,999.99);
    f_gMoment = new TGDoubleEdit(parent,IDC_F_GMOMENT,11,3,0,999.99);
    f_gShear = new TGDoubleEdit(parent,IDC_F_GSHEAR,11,3,0,999.99);

    loc->SetBlankNull(false);
    Ax->SetBlankNull(false);
    Iz->SetBlankNull(false);
    ModE->SetBlankNull(false);
    if (unitWeight)
       unitWeight->SetBlankNull(false);
    if (density)
       density->SetBlankNull(false);
    s_gMoment->SetBlankNull(false);
    s_gShear->SetBlankNull(false);
    f_gMoment->SetBlankNull(false);
    f_gShear->SetBlankNull(false);
/*
    loc->SetValidator(new TExtremeValueValidator(0.00,TExtremeValueValidator::ltGT));
    Ax->SetValidator(new TExtremeValueValidator(0.00,TExtremeValueValidator::ltGT));
    Iz->SetValidator(new TExtremeValueValidator(0.00,TExtremeValueValidator::ltGT));
    ModE->SetValidator(new TExtremeValueValidator(0.00,TExtremeValueValidator::ltGT));
    unitWeight->SetValidator(new TExtremeValueValidator(0.00,TExtremeValueValidator::ltGE));
*/

    Add(loc,"Location", TGrid::Right);
    Add(Ax,"Ax", TGrid::Right);
    Add(Iz,"Iz", TGrid::Right);
    Add(ModE,"Mod E", TGrid::Right);
    if (unitWeight)
       Add(unitWeight,"Unit Wgt", TGrid::Right);
    else
       Add(density,"Density",TGrid::Right);
    Add(s_gMoment,"gM (Str)", TGrid::Right);
    Add(s_gShear,"gV (Str)", TGrid::Right);
    Add(f_gMoment,"gM (Fatg)", TGrid::Right);
    Add(f_gShear,"gV (Fatg)", TGrid::Right);

    lengthEdit = 0; // association with parent dialogs length edit control
                    // needed for validation
}

void SpanTable::SetLengthEdit(TLengthEdit* le)
{
   lengthEdit = le;
}

uint SpanTable::Transfer(void *buffer,TTransferDirection direction)
{
   SpanTableData *data = (SpanTableData *)buffer;

   if (direction == tdGetData)
      *data = table;
   else if (direction == tdSetData)
      {
      table = *data;
      SetNLines(table.NumRows());
      SetMaxLines(table.NumRows());
      ChangeWindowPos (Attr.W, Attr.H, SWP_NOMOVE);
      }

   return sizeof(SpanTableData);
}

const void *SpanTable::TransferItem (int line, int id)
{
   SpanTableData::Row &row = table.GetRow(line);

   switch(id)
      {
      case IDC_LOCATION:   return &(row.loc);
      case IDC_AX:         return &(row.Ax);
      case IDC_IZ:         return &(row.Iz);
      case IDC_MODE:       return &(row.ModE);
      case IDC_UNITWEIGHT:    return &(row.unitWeight);
      case IDC_S_GMOMENT:  return &(row.s_gMoment);
      case IDC_S_GSHEAR:   return &(row.s_gShear);
      case IDC_F_GMOMENT:  return &(row.f_gMoment);
      case IDC_F_GSHEAR:   return &(row.f_gShear);
      }

   return 0;
}

void SpanTable::TransferItem(int line, int id, const void *data)
{
   SpanTableData::Row &row = table.GetRow(line);

   switch(id)
      {
      case IDC_LOCATION:
           row.loc = XFR_DOUBLE(data);
           break;

      case IDC_AX:
           row.Ax = XFR_DOUBLE(data);
           break;

      case IDC_IZ:
           row.Iz = XFR_DOUBLE(data);
           break;

      case IDC_MODE:
           row.ModE = XFR_DOUBLE(data);
           break;

      case IDC_UNITWEIGHT:
           row.unitWeight = XFR_DOUBLE(data);
           break;

      case IDC_S_GMOMENT:
           row.s_gMoment = XFR_DOUBLE(data);
           break;

      case IDC_S_GSHEAR:
           row.s_gShear = XFR_DOUBLE(data);
           break;

      case IDC_F_GMOMENT:
           row.f_gMoment = XFR_DOUBLE(data);
           break;

      case IDC_F_GSHEAR:
           row.f_gShear = XFR_DOUBLE(data);
           break;
      }
}

bool SpanTable::AppendNewLine(int &)
{
   return false;
}

bool SpanTable::InsertLine(int line)
{
   table.InsertRow(line+1,(units == unitsSI));
   SetNLines(GetNLines()+1);
   SetMaxLines(GetMaxLines()+1);
   Go(line+1); // Go to new line
   ChangeWindowPos (Attr.W, Attr.H, SWP_NOMOVE);
   Invalidate();
   UpdateWindow();

   return true;
}

bool SpanTable::DeleteLine(int line)
{
   if (GetNLines() != 1 || GetMaxLines() != 1)
      {
      if (line == GetCurrentLine())
         Go(GetCurrentLine()-1);

      table.DeleteRow(line);

      if (line == GetNLines()-1)
         {
         firstLine--;
         if (firstLine < 0)
            firstLine = 0;
         }
      SetNLines(GetNLines()-1);
      SetMaxLines(GetMaxLines()-1);

      ChangeWindowPos (Attr.W, Attr.H, SWP_NOMOVE);
      Invalidate();
      UpdateWindow();

      return true;
      }
   return false;
}

void SpanTable::ProcessMouse(uint modKeys, TPoint &point,uint message,bool sendToControl)
{
   switch(message)
      {
      case WM_LBUTTONDOWN:
           // This is not called if the newly selected cell and the old
           // cell in are within the same row. This fixes a bug in TGrid
           if (!CanKillFocus(current))
              return;
           break;

      case WM_RBUTTONDOWN:
           TPopupMenu menu;

           ClientToScreen(point);

           menu.AppendMenu(MF_BYCOMMAND,CM_INSERT,"Insert Row");
           menu.AppendMenu(MF_BYCOMMAND,CM_DELETE,"Delete Row");
           menu.AppendMenu(MF_BYCOMMAND,CM_DUPLICATE,"Copy Row");
           menu.AppendMenu(MF_SEPARATOR);
           menu.AppendMenu(MF_BYCOMMAND,CM_CLEARALL,"Clear All");

           menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point,0,HWindow);
           break;
      }

   TGrid::ProcessMouse(modKeys,point,message,sendToControl);
}


void SpanTable::CmInsert()
{
   VkInsert(true,0,1);
}

void SpanTable::CmDelete()
{
   VkDelete(true,0,1);
}

void SpanTable::CeDelete(TCommandEnabler& tce)
{
   if (GetCurrentLine() == 0)
      tce.Enable(false);
}

void SpanTable::CmDuplicate()
{
   table.CopyRow(GetCurrentLine());
   SetNLines(GetNLines()+1);
   SetMaxLines(GetMaxLines()+1);
   Go(GetNLines());
   ChangeWindowPos (Attr.W, Attr.H, SWP_NOMOVE);
}

void SpanTable::CmClearAll()
{
   Go(0);
   table.ClearAll((units == unitsSI));
   SetNLines(table.NumRows());
   SetMaxLines(table.NumRows());
   ChangeWindowPos (Attr.W, Attr.H, SWP_NOMOVE);
   Invalidate();
   UpdateWindow();
}

void SpanTable::Sort()
{
   table.Sort();
   Invalidate();   // Invalidate the entire grid
   UpdateWindow(); // Force the window to paint itself now!!!
}

TGrid::Visible SpanTable::VisibleCell(int line,int id)
{
   if (line == 0 && id == IDC_LOCATION)
      return gLocked;
   else
      return TGrid::Visible(line,id);
}


bool SpanTable::CanClose()
{
   bool retval;

   retval = TGrid::CanClose();

   if (retval)
      retval = ValidateTable();

   if (retval)
      Sort();

   return retval;
}

bool SpanTable::ValidateTable()
{
//
// Validation of the table could be way better than this.
// Each cell should be validated as it loses focus. The
// validation stuff in TGrid doesn't work all that well, so this
// is a reasonable workaround.
//
// When time permits, clean this up!!!
//

   int r1,r2,numRows;
   SpanTableData::Row *row1, *row2;
   double spanLength;

   spanLength = lengthEdit->GetValue();
   
   numRows = table.NumRows();
   for (r1 = 0; r1 < numRows; r1++)
      {
      row1 = (*table.rows)[r1];

      // Check all cells in this row
      if (r1 != 0)
         {
         if (!(row1->loc > 0))
            {
            ostrstream os;
            os << "Location must be greater that zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }
         }

      if (row1->loc >= spanLength)
         {
         ostrstream os;
         os << "Location must be less than span length in Row " << (r1+1) << ends;
         MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
         delete[] os.str();
         return false;
         }

      if (!(row1->Ax > 0))
            {
            ostrstream os;
            os << "Ax must be greater that zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }

      if (!(row1->Iz > 0))
            {
            ostrstream os;
            os << "Iz must be greater that zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }

      if (!(row1->ModE > 0))
            {
            ostrstream os;
            os << "Mod E must be greater that zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }

      if (row1->unitWeight < 0)
            {
            ostrstream os;
            if (units == unitsSI)
               os << "Density must be greater that or equal to zero in Row " << (r1+1) << ends;
            else
               os << "Unit Wgt must be greater that or equal to zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }

      if (!(row1->s_gMoment > 0))
            {
            ostrstream os;
            os << "gM (Str) must be greater that zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }

      if (!(row1->s_gShear > 0))
            {
            ostrstream os;
            os << "gV (Str) must be greater that zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }

      if (!(row1->f_gMoment > 0))
            {
            ostrstream os;
            os << "gM (Fatg) must be greater that zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }

      if (!(row1->f_gShear > 0))
            {
            ostrstream os;
            os << "gV (Fatg) must be greater that zero in Row " << (r1+1) << ends;
            MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
            delete[] os.str();
            return false;
            }

      // Check for duplicate locations
      for (r2 = r1+1; r2 < numRows; r2++)
         {
         row2 = (*table.rows)[r2];

         if (r1 != r2)
            {
            if (row1->loc == row2->loc)
               {
               ostrstream os;
               os << "Row " << (r1+1) << " and " << (r2+1) << " have the same location" << ends;
               MessageBox(os.str(),"Error",MB_OK | MB_ICONEXCLAMATION);
               return false;
               }
            }
         }
      }

   return true;
}
