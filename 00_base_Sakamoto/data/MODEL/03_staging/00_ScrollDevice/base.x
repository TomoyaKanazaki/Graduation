xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 32;
 -247.89956;51.57083;-210.19688;,
 247.89986;51.57083;-210.19688;,
 247.89986;-3.94343;-210.19688;,
 -247.89956;-3.94343;-210.19688;,
 247.89986;122.14068;207.78457;,
 -247.89956;122.14068;207.78457;,
 -247.89956;-3.94343;207.78457;,
 247.89986;-3.94343;207.78457;,
 247.89986;122.14068;160.75738;,
 247.89986;122.14068;207.78457;,
 247.89986;-3.94343;207.78457;,
 247.89986;-3.94343;160.75738;,
 247.89986;-3.94343;-210.19688;,
 247.89986;51.57083;-210.19688;,
 247.89986;122.14068;160.75738;,
 247.89986;51.57083;-210.19688;,
 -247.89956;51.57083;-210.19688;,
 -247.89956;122.14068;160.75738;,
 247.89986;-3.94343;160.75738;,
 -247.89956;-3.94343;207.78457;,
 -247.89956;-3.94343;160.75738;,
 -247.89956;-3.94343;-210.19688;,
 247.89986;-3.94343;-210.19688;,
 -247.89956;122.14068;160.75738;,
 -247.89956;51.57083;-210.19688;,
 -247.89956;-3.94343;-210.19688;,
 -247.89956;-3.94343;160.75738;,
 -247.89956;122.14068;207.78457;,
 -168.84525;794.74508;160.75738;,
 168.84555;794.74508;160.75738;,
 -168.84525;794.74508;207.78457;,
 168.84555;794.74508;207.78457;;
 
 14;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,12,13;,
 4;14,15,16,17;,
 4;18,10,19,20;,
 4;18,20,21,22;,
 4;23,24,25,26;,
 4;23,26,19,27;,
 4;28,29,14,17;,
 4;30,28,17,27;,
 4;31,30,27,9;,
 4;29,31,9,14;,
 4;29,28,30,31;;
 
 MeshMaterialList {
  4;
  14;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.034400;0.000000;1.000000;;
   5.000000;
   0.250000;0.250000;0.250000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.270000;0.270000;0.270000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\SlopeDevice\\line.jpg";
   }
  }
  Material {
   0.800000;0.583200;0.000000;1.000000;;
   5.000000;
   0.440000;0.440000;0.440000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  13;
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.999240;0.038970;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.982381;-0.186887;,
  0.000000;-1.000000;-0.000000;,
  -0.993164;0.116731;0.000000;,
  0.998289;0.058466;0.000000;,
  -0.999240;0.038970;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.998289;0.058466;0.000000;,
  0.993164;0.116731;0.000000;,
  0.000000;1.000000;0.000000;;
  14;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,7,3,3;,
  4;2,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;8,9,9,9;,
  4;8,9,9,10;,
  4;0,0,0,0;,
  4;6,6,8,10;,
  4;1,1,1,1;,
  4;11,11,7,2;,
  4;12,12,12,12;;
 }
 MeshTextureCoords {
  32;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.887490;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.887490;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.112510;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.112510;,
  1.000000;0.887490;,
  0.000000;1.000000;,
  0.000000;0.887490;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.112510;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.112510;1.000000;,
  0.000000;0.000000;,
  0.000000;0.112510;,
  1.000000;0.112510;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}