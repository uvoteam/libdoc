#!/usr/bin/python3

from distutils.core import setup, Extension

setup(name = 'extract_doc',
      version = '1.0',
      ext_modules = [Extension('extract_doc', sources = ['extract_doc.c'], libraries = ['doc'])])

