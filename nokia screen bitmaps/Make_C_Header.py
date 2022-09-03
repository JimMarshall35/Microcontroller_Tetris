
import png

def AddByteFromVerticalSlice(outputArray, x, y, rawData, pixelStride=4):
    outputByte = 0x00
    pixelStartXIndex = x * pixelStride
    for rowAddition in range(8):
        row = list(rawData[y+rowAddition])     
        if(row[pixelStartXIndex] != 0x00):
            outputByte |= (1 << rowAddition)
    outputArray.append(outputByte)
    
filepath = "TetrisTitleInverted.png"

rawData = []
try:
    r = png.Reader(filename=filepath)
    print(r)
    rawData = r.read()
except IOError:
    print('Error While Opening the file!')

print(rawData)

pixelsWidth = rawData[0]
pixelsHeight = rawData[1]

metaData = rawData[3]
rawBytes = list(rawData[2])
metaData = rawData[3]
pixelSize = metaData["planes"]
processedBytes = []

for y in range(0,pixelsHeight,8):
    for x in range(0, pixelsWidth):
        AddByteFromVerticalSlice(processedBytes, x, y, rawBytes)
    print(x,y)

cHeaderString = "static u8 picture["+str(len(processedBytes))+"] = { "
for byte in processedBytes:
    cHeaderString += str(byte) + ", "
cHeaderString += "};"
print(cHeaderString)
