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
 86;
 14.00000;83.80000;0.60000;,
 -13.85714;83.80000;0.60003;,
 -13.85714;1.20000;0.60003;,
 14.00000;1.20000;0.60000;,
 -13.85714;83.80000;-0.60018;,
 14.00000;83.80000;-0.60020;,
 14.00000;1.20000;-0.60020;,
 -13.85714;1.20000;-0.60018;,
 -10.49665;38.66694;-0.73500;,
 -11.23070;38.32640;-0.73500;,
 -11.23070;38.32640;0.73491;,
 -10.49665;38.66694;0.73491;,
 -11.23070;38.32640;-0.73500;,
 -11.53477;37.50425;-0.73500;,
 -11.53477;37.50425;0.73491;,
 -11.23070;38.32640;0.73491;,
 -11.23070;36.68210;-0.73500;,
 -11.23070;36.68210;0.73491;,
 -10.49665;36.34155;-0.73500;,
 -10.49665;36.34155;0.73491;,
 -9.76258;36.68210;-0.73500;,
 -9.76258;36.68210;0.73491;,
 -9.45853;37.50425;-0.73501;,
 -9.45853;37.50425;0.73491;,
 -9.76258;38.32640;-0.73500;,
 -9.76258;38.32640;0.73491;,
 -10.49665;38.66694;-0.73500;,
 -10.49665;38.66694;0.73491;,
 -10.49665;37.50425;-0.62659;,
 -10.49665;38.66694;-0.73500;,
 -10.49665;37.50425;-0.62659;,
 -10.49665;37.50425;-0.62659;,
 -10.49665;37.50425;-0.62659;,
 -10.49665;37.50425;-0.62659;,
 -10.49665;37.50425;-0.62659;,
 -10.49665;37.50425;-0.62659;,
 -10.49665;37.50425;-0.62659;,
 -10.49665;37.50425;0.62650;,
 -10.49665;38.66694;0.73491;,
 -10.49665;37.50425;0.62650;,
 -10.49665;37.50425;0.62650;,
 -10.49665;37.50425;0.62650;,
 -10.49665;37.50425;0.62650;,
 -10.49665;37.50425;0.62650;,
 -10.49665;37.50425;0.62650;,
 -10.49665;37.50425;0.62650;,
 -14.92857;85.00000;-0.60018;,
 15.07143;85.00000;-0.60020;,
 14.00000;83.80000;-0.60020;,
 -13.85714;83.80000;-0.60018;,
 -14.92857;85.00000;0.60003;,
 -14.92857;85.00000;-0.60018;,
 -13.85714;83.80000;-0.60018;,
 -13.85714;83.80000;0.60003;,
 15.07143;85.00000;0.60000;,
 -14.92857;85.00000;0.60003;,
 -13.85714;83.80000;0.60003;,
 14.00000;83.80000;0.60000;,
 15.07143;85.00000;-0.60020;,
 15.07143;85.00000;0.60000;,
 14.00000;83.80000;0.60000;,
 14.00000;83.80000;-0.60020;,
 -14.92857;0.00000;-0.60018;,
 -13.85714;1.20000;-0.60018;,
 -14.92857;0.00000;0.60003;,
 -14.92857;0.00000;-0.60018;,
 -13.85714;1.20000;-0.60018;,
 -13.85714;1.20000;0.60003;,
 -14.92857;85.00000;0.60003;,
 -14.92857;0.00000;0.60003;,
 -13.85714;1.20000;0.60003;,
 -13.85714;83.80000;0.60003;,
 15.07143;0.00000;0.60000;,
 15.07143;85.00000;0.60000;,
 14.00000;83.80000;0.60000;,
 14.00000;1.20000;0.60000;,
 15.07143;0.00000;-0.60020;,
 15.07143;0.00000;0.60000;,
 14.00000;1.20000;0.60000;,
 14.00000;1.20000;-0.60020;,
 15.07143;0.00000;-0.60020;,
 14.00000;1.20000;-0.60020;,
 -14.92857;0.00000;0.60003;,
 15.07143;0.00000;0.60000;,
 14.00000;1.20000;0.60000;,
 -13.85714;1.20000;0.60003;;
 
 46;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;13,16,17,14;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 3;28,12,29;,
 3;30,13,12;,
 3;31,16,13;,
 3;32,18,16;,
 3;33,20,18;,
 3;34,22,20;,
 3;35,24,22;,
 3;36,26,24;,
 3;37,38,15;,
 3;39,15,14;,
 3;40,14,17;,
 3;41,17,19;,
 3;42,19,21;,
 3;43,21,23;,
 3;44,23,25;,
 3;45,25,27;,
 4;46,47,48,49;,
 4;50,51,52,53;,
 4;54,55,56,57;,
 4;58,59,60,61;,
 4;47,46,55,54;,
 4;51,50,53,52;,
 4;62,46,49,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;68,46,62,69;,
 4;59,58,61,60;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;47,80,81,48;,
 4;47,73,72,80;,
 4;82,83,84,85;,
 4;65,64,67,66;,
 4;80,62,63,81;,
 4;77,76,79,78;,
 4;83,82,62,80;;
 
 MeshMaterialList {
  6;
  46;
  2,
  2,
  1,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  1,
  4,
  1,
  4,
  1,
  4,
  1,
  4,
  4,
  1,
  4,
  1,
  4,
  4,
  4,
  1,
  4,
  1,
  4;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MapObj\\fusuma002.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MapObj\\fusuma002.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MapObj\\fusuma002.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MapObj\\fusuma002.jpg";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
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
  50;
  0.000001;0.000000;1.000000;,
  0.000001;0.000000;1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;0.000000;-1.000000;,
  -0.000002;-0.000000;-1.000000;,
  -0.000005;1.000000;0.000000;,
  -0.734859;0.678220;0.000000;,
  -1.000000;0.000002;0.000000;,
  -0.734862;-0.678217;0.000000;,
  -0.000005;-1.000000;0.000000;,
  0.734865;-0.678213;-0.000000;,
  1.000000;0.000002;0.000000;,
  0.734863;0.678216;0.000000;,
  0.000000;-0.000000;1.000000;,
  0.000001;0.000000;1.000000;,
  -0.934328;-0.356415;0.000000;,
  -0.000000;-0.000000;-1.000000;,
  0.408646;-0.912693;-0.000000;,
  -0.000000;0.000000;-1.000000;,
  0.000001;0.000000;1.000000;,
  -0.408647;-0.912693;0.000000;,
  0.073470;-0.065623;-0.995136;,
  0.000001;-0.092838;-0.995681;,
  0.103864;-0.000000;-0.994591;,
  0.073470;0.065623;-0.995136;,
  0.000001;0.092837;-0.995681;,
  -0.073474;0.065617;-0.995136;,
  -0.103874;-0.000000;-0.994590;,
  -0.073474;-0.065618;-0.995136;,
  0.000001;-0.092838;0.995681;,
  0.073470;-0.065623;0.995136;,
  0.103864;-0.000000;0.994591;,
  0.073470;0.065623;0.995136;,
  0.000001;0.092837;0.995681;,
  -0.073469;0.065621;0.995136;,
  -0.103864;-0.000000;0.994591;,
  -0.073469;-0.065622;0.995136;,
  -0.745937;-0.666017;0.000000;,
  0.934328;-0.356414;-0.000000;,
  0.745937;-0.666016;-0.000000;,
  -0.408647;0.912693;0.000000;,
  0.408646;0.912693;0.000000;,
  0.745937;0.666017;0.000000;,
  0.745937;-0.666017;-0.000000;,
  -0.934328;0.356415;0.000000;,
  -0.745937;0.666016;0.000000;,
  -0.745937;-0.666016;0.000000;,
  0.934328;0.356414;0.000000;,
  -0.745937;0.666017;0.000000;,
  0.745937;0.666016;0.000000;;
  46;
  4;0,0,1,1;,
  4;2,2,3,3;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,5,5,12;,
  3;4,21,22;,
  3;4,23,21;,
  3;4,24,23;,
  3;4,25,24;,
  3;4,26,25;,
  3;4,27,26;,
  3;4,28,27;,
  3;4,22,28;,
  3;13,29,30;,
  3;13,30,31;,
  3;13,31,32;,
  3;13,32,33;,
  3;13,33,34;,
  3;13,34,35;,
  3;13,35,36;,
  3;13,36,29;,
  4;16,16,2,2;,
  4;15,15,37,37;,
  4;14,14,0,0;,
  4;38,38,39,39;,
  4;40,41,41,40;,
  4;41,41,42,42;,
  4;18,16,2,3;,
  4;17,17,43,43;,
  4;14,19,1,0;,
  4;15,15,44,44;,
  4;40,40,45,45;,
  4;19,14,0,1;,
  4;20,20,46,46;,
  4;16,18,3,2;,
  4;38,38,47,47;,
  4;19,19,1,1;,
  4;44,44,48,48;,
  4;18,18,3,3;,
  4;47,47,49,49;,
  4;20,17,17,20;;
 }
 MeshTextureCoords {
  86;
  0.746040;0.007861;,
  0.510109;0.007861;,
  0.510109;0.999731;,
  0.746040;0.999731;,
  0.510109;0.007861;,
  0.746040;0.007861;,
  0.746040;0.999731;,
  0.510109;0.999731;,
  0.295868;0.545135;,
  0.289651;0.549183;,
  0.289651;0.549183;,
  0.295868;0.545135;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.125000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.258333;-0.005715;,
  0.258333;-0.005715;,
  0.267407;0.008551;,
  0.267407;0.008551;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.512413;-0.005715;,
  0.512413;-0.005715;,
  0.503339;0.008551;,
  0.503339;0.008551;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.258333;1.004843;,
  0.258333;1.004843;,
  0.267407;0.990576;,
  0.267407;0.990576;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.512413;1.004843;,
  0.512413;1.004843;,
  0.503339;0.990576;,
  0.503339;0.990576;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}