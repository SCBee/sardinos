' Copyright (c) 2021 Lockheed Martin Corp. All rights reserved.

Const ForReading = 1
Const ForWriting = 2

fileName = Wscript.Arguments(0)
oldText = Wscript.Arguments(1)
newText = Wscript.Arguments(2)

Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile(fileName, ForReading)

fileContent = objFile.ReadAll
objFile.Close
newFileContent = Replace(fileContent, oldText, newText)

Set objFile = objFSO.OpenTextFile(fileName, ForWriting)
objFile.Write newFileContent
objFile.Close
