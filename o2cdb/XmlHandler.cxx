//  The SAX XML file handler used in the CDBManager                       //
#include <cstdlib>
#include <Riostream.h>

#include <TList.h>
#include <TObject.h>
#include <TXMLAttr.h>
#include <TSAXParser.h>

#include <FairLogger.h>
#include "XmlHandler.h"

using namespace AliceO2::CDB;
ClassImp(XmlHandler)

XmlHandler::XmlHandler() : TObject(), mRun(-1), mStartIdRunRange(-1), mEndIdRunRange(-1), mOCDBFolder("")
{
  //
  // XmlHandler default constructor
  //
}

XmlHandler::XmlHandler(Int_t run) : TObject(), mRun(run), mStartIdRunRange(-1), mEndIdRunRange(-1), mOCDBFolder("")
{
  //
  // XmlHandler constructor with requested run
  //
}

XmlHandler::XmlHandler(const XmlHandler& sh)
  : TObject(sh),
    mRun(sh.mRun),
    mStartIdRunRange(sh.mStartIdRunRange),
    mEndIdRunRange(sh.mEndIdRunRange),
    mOCDBFolder(sh.mOCDBFolder)
{
  //
  // XmlHandler copy constructor
  //
}

XmlHandler& XmlHandler::operator=(const XmlHandler& sh)
{
  //
  // Assignment operator
  //
  if (&sh == this)
    return *this;

  new (this) XmlHandler(sh);
  return *this;
}

XmlHandler::~XmlHandler()
{
  //
  // XmlHandler destructor
  //
}

void XmlHandler::OnStartDocument()
{
  // if something should happen right at the beginning of the
  // XML document, this must happen here
  LOG(INFO) << "Reading XML file for LHCPeriod <-> Run Range correspondence" << FairLogger::endl;
}

void XmlHandler::OnEndDocument()
{
  // if something should happen at the end of the XML document
  // this must be done here
}

void XmlHandler::OnStartElement(const char* name, const TList* attributes)
{
  // when a new XML element is found, it is processed here

  // set the current system if necessary
  TString strName(name);
  LOG(DEBUG) << "name = " << strName.Data() << FairLogger::endl;
  Int_t startRun = -1;
  Int_t endRun = -1;
  TXMLAttr* attr;
  TIter next(attributes);
  while ((attr = (TXMLAttr*)next())) {
    TString attrName = attr->GetName();
    LOG(DEBUG) << "Name = " << attrName.Data() << FairLogger::endl;
    if (attrName == "StartIdRunRange") {
      startRun = (Int_t)(((TString)(attr->GetValue())).Atoi());
      LOG(DEBUG) << "startRun = " << startRun << FairLogger::endl;
    }
    if (attrName == "EndIdRunRange") {
      endRun = (Int_t)(((TString)(attr->GetValue())).Atoi());
      LOG(DEBUG) << "endRun = " << endRun << FairLogger::endl;
    }
    if (attrName == "OCDBFolder") {
      if (mRun >= startRun && mRun <= endRun && startRun != -1 && endRun != -1) {
        mOCDBFolder = (TString)(attr->GetValue());
        LOG(DEBUG) << "OCDBFolder = " << mOCDBFolder.Data() << FairLogger::endl;
        mStartIdRunRange = startRun;
        mEndIdRunRange = endRun;
      }
    }
  }
  return;
}
void XmlHandler::OnEndElement(const char* name)
{
  // do everything that needs to be done when an end tag of an element is found
  TString strName(name);
  LOG(DEBUG) << "name = " << strName.Data() << FairLogger::endl;
}

void XmlHandler::OnCharacters(const char* characters)
{
  // copy the text content of an XML element
  // mContent = characters;
  TString strCharacters(characters);
  LOG(DEBUG) << "characters = " << strCharacters.Data() << FairLogger::endl;
}

void XmlHandler::OnComment(const char* /*text*/)
{
  // comments within the XML file are ignored
}

void XmlHandler::OnWarning(const char* text)
{
  // process warnings here
  LOG(INFO) << "Warning: " << text << FairLogger::endl;
}

void XmlHandler::OnError(const char* text)
{
  // process errors here
  LOG(ERROR) << "Error: " << text << FairLogger::endl;
}

void XmlHandler::OnFatalError(const char* text)
{
  // process fatal errors here
  LOG(FATAL) << "Fatal error: " << text << FairLogger::endl;
}

void XmlHandler::OnCdataBlock(const char* /*text*/, Int_t /*len*/)
{
  // process character data blocks here
  // not implemented and should not be used here
}
