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
 68;
 11.61110;16.56486;-31.02740;,
 7.70302;24.65214;-26.16822;,
 6.27122;4.72994;4.20740;,
 9.58621;-2.12977;0.08562;,
 -1.73188;28.00190;-24.15533;,
 -1.73188;7.57167;5.91492;,
 -11.16677;24.65214;-26.16822;,
 -9.73497;4.72994;4.20740;,
 -15.07486;16.56486;-31.02740;,
 -13.04996;-2.12977;0.08562;,
 -11.16677;8.47739;-35.88678;,
 -9.73497;-8.98981;-4.03616;,
 -1.73188;5.12769;-37.89957;,
 -1.73188;-11.83134;-5.74348;,
 7.70301;8.47739;-35.88678;,
 6.27122;-8.98981;-4.03616;,
 11.61110;16.56486;-31.02740;,
 9.58621;-2.12977;0.08562;,
 -1.73188;16.56486;-31.02740;,
 -1.73188;16.56486;-31.02740;,
 -1.73188;16.56486;-31.02740;,
 -1.73188;16.56486;-31.02740;,
 -1.73188;16.56486;-31.02740;,
 -1.73188;16.56486;-31.02740;,
 -1.73188;16.56486;-31.02740;,
 -1.73188;16.56486;-31.02740;,
 -1.73188;-2.12977;0.08562;,
 -1.73188;-2.12977;0.08562;,
 -1.73188;-2.12977;0.08562;,
 -1.73188;-2.12977;0.08562;,
 -1.73188;-2.12977;0.08562;,
 -1.73188;-2.12977;0.08562;,
 -1.73188;-2.12977;0.08562;,
 -1.73188;-2.12977;0.08562;,
 9.87198;87.45978;-148.60640;,
 6.47327;94.49286;-144.38052;,
 6.47327;21.85189;-23.48532;,
 9.87198;14.81865;-27.71119;,
 -1.73188;97.40612;-142.63013;,
 -1.73188;24.76500;-21.73483;,
 -9.93702;94.49286;-144.38052;,
 -9.93702;21.85189;-23.48532;,
 -13.33572;87.45978;-148.60640;,
 -13.33572;14.81865;-27.71119;,
 -9.93702;80.42667;-152.83245;,
 -9.93702;7.78547;-31.93707;,
 -1.73188;77.51333;-154.58294;,
 -1.73188;4.87217;-33.68756;,
 6.47327;80.42667;-152.83245;,
 6.47327;7.78547;-31.93707;,
 9.87198;87.45978;-148.60640;,
 9.87198;14.81865;-27.71119;,
 -1.73188;87.45978;-148.60640;,
 -1.73188;87.45978;-148.60640;,
 -1.73188;87.45978;-148.60640;,
 -1.73188;87.45978;-148.60640;,
 -1.73188;87.45978;-148.60640;,
 -1.73188;87.45978;-148.60640;,
 -1.73188;87.45978;-148.60640;,
 -1.73188;87.45978;-148.60640;,
 -1.73188;14.81865;-27.71119;,
 -1.73188;14.81865;-27.71119;,
 -1.73188;14.81865;-27.71119;,
 -1.73188;14.81865;-27.71119;,
 -1.73188;14.81865;-27.71119;,
 -1.73188;14.81865;-27.71119;,
 -1.73188;14.81865;-27.71119;,
 -1.73188;14.81865;-27.71119;;
 
 48;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 3;52,35,34;,
 3;53,38,35;,
 3;54,40,38;,
 3;55,42,40;,
 3;56,44,42;,
 3;57,46,44;,
 3;58,48,46;,
 3;59,50,48;,
 3;60,37,36;,
 3;61,36,39;,
 3;62,39,41;,
 3;63,41,43;,
 3;64,43,45;,
 3;65,45,47;,
 3;66,47,49;,
 3;67,49,51;;
 
 MeshMaterialList {
  5;
  48;
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
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
  4;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\SlopeDevice\\dodai001.jpg";
   }
  }
  Material {
   0.800000;0.790588;0.765490;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\SlopeDevice\\Iron000.png";
   }
  }
  Material {
   0.743529;0.263529;0.194510;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.059608;1.000000;;
   5.000000;
   0.100000;0.100000;0.100000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.110000;0.110000;0.110000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  50;
  0.000000;0.515034;-0.857170;,
  0.998448;-0.028683;0.047747;,
  0.706011;0.576479;0.411365;,
  -0.000000;0.827148;0.561984;,
  -0.706011;0.576478;0.411366;,
  -0.998448;-0.028683;0.047748;,
  -0.706008;-0.633857;-0.315877;,
  -0.000000;-0.884525;-0.466493;,
  0.706008;-0.633857;-0.315878;,
  0.000000;-0.515034;0.857170;,
  0.000000;0.515041;-0.857166;,
  1.000000;0.000001;0.000000;,
  0.707103;0.606113;0.364188;,
  -0.000000;0.857168;0.515037;,
  -0.707103;0.606112;0.364188;,
  -1.000000;0.000001;0.000000;,
  -0.707109;-0.606107;-0.364185;,
  -0.000000;-0.857168;-0.515038;,
  0.707108;-0.606108;-0.364186;,
  -0.000000;-0.515032;0.857171;,
  -0.000007;0.515032;-0.857171;,
  0.000000;0.515030;-0.857172;,
  0.000000;0.515038;-0.857167;,
  0.000007;0.515032;-0.857171;,
  -0.000004;0.515037;-0.857168;,
  0.000000;0.515040;-0.857166;,
  0.000004;0.515037;-0.857168;,
  0.000000;-0.515033;0.857170;,
  0.000000;-0.515043;0.857164;,
  -0.000000;-0.515044;0.857164;,
  -0.000000;-0.515043;0.857164;,
  0.000000;-0.515033;0.857170;,
  0.000001;-0.515025;0.857175;,
  0.000000;-0.515026;0.857175;,
  -0.000001;-0.515025;0.857175;,
  0.000002;0.515034;-0.857170;,
  0.000000;0.515043;-0.857164;,
  -0.000000;0.515032;-0.857171;,
  -0.000002;0.515034;-0.857170;,
  0.000000;0.515043;-0.857164;,
  0.000003;0.515048;-0.857162;,
  0.000000;0.515045;-0.857163;,
  -0.000003;0.515048;-0.857162;,
  0.000000;-0.515029;0.857173;,
  0.000006;-0.515032;0.857171;,
  -0.000000;-0.515038;0.857168;,
  -0.000006;-0.515032;0.857171;,
  0.000002;-0.515032;0.857171;,
  0.000000;-0.515033;0.857170;,
  -0.000002;-0.515032;0.857171;;
  48;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,20,21;,
  3;0,22,20;,
  3;0,23,22;,
  3;0,21,23;,
  3;0,24,21;,
  3;0,25,24;,
  3;0,26,25;,
  3;0,21,26;,
  3;9,27,28;,
  3;9,28,29;,
  3;9,29,30;,
  3;9,30,31;,
  3;9,31,32;,
  3;9,32,33;,
  3;9,33,34;,
  3;9,34,27;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,18,18,17;,
  4;18,11,11,18;,
  3;10,35,36;,
  3;10,37,35;,
  3;10,38,37;,
  3;10,39,38;,
  3;10,40,39;,
  3;10,41,40;,
  3;10,42,41;,
  3;10,36,42;,
  3;19,43,44;,
  3;19,44,45;,
  3;19,45,46;,
  3;19,46,43;,
  3;19,43,47;,
  3;19,47,48;,
  3;19,48,49;,
  3;19,49,43;;
 }
 MeshTextureCoords {
  68;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
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
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
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
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}