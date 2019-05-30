#!/usr/bin/python3

import extract_doc

with open('./test.doc', 'rb') as myfile:
      data = bytearray(myfile.read())
      print(extract_doc.extract_doc_text(data, len(data)))
