#!/usr/bin/python3

import extract_doc
import importlib
importlib.reload(extract_doc)
with open('./test3.doc', 'rb') as myfile:
      data = bytearray(myfile.read())
      print(extract_doc.extract_doc_text(data, len(data)))
