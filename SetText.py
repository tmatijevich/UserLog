import csv
import ctypes
import xml.etree.ElementTree as ET

InputFile = 'Text.csv'
OutputFile = 'UserLog.tmx'

def IndentXML(Element, Level = 0):
    Tab = '  '
    i = "\n" + Level * Tab
    if len(Element):
        if not Element.text or not Element.text.strip():
            Element.text = i + Tab
        if not Element.tail or not Element.tail.strip():
            Element.tail = i
        for Element in Element:
            IndentXML(Element, Level+1)
        if not Element.tail or not Element.tail.strip():
            Element.tail = i
    else:
        if Level and (not Element.tail or not Element.tail.strip()):
            Element.tail = i

BaseFile = '''<?xml version="1.0" encoding="utf-8"?>
<tmx version="1.4">
  <header creationtool="B&amp;R Automation Studio" creationtoolversion="4.2" datatype="unknown" segtype="sentence" adminlang="en" srclang="en" o-tmf="TMX">
    <note>Change the namespace to define where this text module should be located within the logical structure of your texts</note>
    <prop type="x-BR-TS:Namespace">User/EventLog</prop>
  </header>
  <body />
</tmx>
'''

Root = ET.fromstring(BaseFile)

with open(InputFile, mode = 'r', encoding = 'utf-8-sig') as OpenFile:
    DataCSV = csv.reader(OpenFile)
    next(DataCSV)
    for Row in DataCSV:
        if Row[0] == '':
            print('Empty row, conversion aborted')
            break
        print(Row)
        EventID = ctypes.c_long((int(Row[0]) << 30) + (1 << 29) + (int(Row[1]) << 16) + int(Row[2]))
        print(str(EventID.value))
        
        ElementTu = ET.Element('tu', attrib={'tuid':str(EventID.value)})
        ElementNote = ET.Element('note')
        ElementNote.text = Row[2] + ' ' + Row[4]
        ElementTuv = ET.Element('tuv', attrib={'xml:lang':'en'})
        ElementSeg = ET.Element('seg')
        ElementSeg.text = Row[3]

        ElementTuv.append(ElementSeg)
        ElementTu.append(ElementNote)
        ElementTu.append(ElementTuv)

        Root[1].append(ElementTu)

IndentXML(Root)
ET.dump(Root)
EventData = ET.tostring(Root, encoding='unicode')

LogTextFile = open(OutputFile, 'w', encoding='utf-8-sig')
LogTextFile.write(EventData)
LogTextFile.close()
