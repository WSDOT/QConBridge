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

#if !defined (SPANTBL_H)
#define SPANTBL_H

#include <classlib\arrays.h>
#include <qconbrdg\qcbapp.rh>            // Definition of all resources.
#include <qconbrdg\guniedit.h>
#include <qconbrdg\gmatedit.h>
#include <qconbrdg\mytypes.h>
#include <griddll.h>

// Transfer buffer for SpanTable
struct SpanTableData
{
   // Data structure for a row
   struct Row
      {
      Row(bool isSI);
      Row(const Row &r);

      double loc;
      double Ax;
      double Iz;
      double ModE;
      double unitWeight;
      double s_gMoment;
      double s_gShear;
      double f_gMoment;
      double f_gShear;
      bool operator == (const Row &r) const {return loc == r.loc;}
      bool operator <  (const Row &r) const {return loc <  r.loc;}
      };

   typedef TIArrayAsVector<Row>         RowContainer;
   typedef TIArrayAsVectorIterator<Row> RowIterator;
   RowContainer *rows;

   SpanTableData() {rows = new RowContainer(10,0,10);AppendRow(true);}
   ~SpanTableData() { delete rows;}
   int NumRows() {return rows->GetItemsInContainer();}
   Row &GetRow(int row) {return (*(*rows)[row]);}
   void InsertRow(int afterRow,bool isSI) {rows->AddAt(new Row(isSI),afterRow);}
   void AppendRow(bool isSI) {rows->Add(new Row(isSI));}
   void DeleteRow(int row) {Row *oldRow = (*rows)[row];
                            rows->RemoveEntry(row);
                            delete oldRow;
                            }
   void CopyRow(int row) {rows->Add(new Row(*(*rows)[row]));}
   void ClearAll(bool isSI) {rows->Flush();rows->Add(new Row(isSI));}
   void Sort();

   SpanTableData &operator = (SpanTableData &table);
};

class SpanTable : public TGrid
{
public:
   SpanTable(TWindow *parent,SystemOfUnits units,int resId,TModule *module = 0);

   uint Transfer(void *buffer,TTransferDirection direction);
   const void *TransferItem (int line, int id);
   void TransferItem        (int line, int id, const void *data);
   bool AppendNewLine(int &newLine);
   bool InsertLine(int line);
   bool DeleteLine(int line);
   void ProcessMouse(uint modKeys, TPoint &point,uint message,bool sendToControl);
   void Sort();
   void CmInsert();
   void CmDelete();
   void CeDelete(TCommandEnabler& tce);
   void CmDuplicate();
   void CmClearAll();
   bool CanClose();

   void SetLengthEdit(TLengthEdit* le);

protected:
   TGrid::Visible VisibleCell(int line,int id);

private:
   SpanTableData table;
   TGLengthEdit *loc;
   TGLength2Edit *Ax;
   TGLength4Edit *Iz;
   TGPressureEdit *ModE;
   TGUnitWeightEdit *unitWeight;
   TGMaterialDensityEdit *density;
   TGDoubleEdit *s_gMoment;
   TGDoubleEdit *s_gShear;
   TGDoubleEdit *f_gMoment;
   TGDoubleEdit *f_gShear;

   TLengthEdit* lengthEdit;

   SystemOfUnits units;

   bool ValidateTable();

DECLARE_RESPONSE_TABLE(SpanTable);
};

#endif // SPANTBL_H