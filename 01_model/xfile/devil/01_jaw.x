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
 83;
 2.87213;0.20762;-5.39376;,
 3.19729;0.20762;-2.89407;,
 1.72935;-0.75015;-2.89407;,
 1.22219;-0.75015;-4.24690;,
 2.26019;0.20762;-0.39438;,
 1.22219;-0.75015;-1.54124;,
 -0.00220;0.20762;0.64103;,
 -0.00220;-0.75015;-0.98089;,
 -2.26458;0.20762;-0.39438;,
 -1.22659;-0.75015;-1.54124;,
 -3.20169;0.20762;-2.89407;,
 -1.73375;-0.75015;-2.89407;,
 -2.87653;0.20762;-5.39376;,
 -1.22659;-0.75015;-4.24690;,
 -0.00220;-1.45184;-4.80725;,
 -0.00220;-1.22647;-2.89407;,
 -0.00220;-1.22647;-2.89407;,
 -0.00220;-1.22647;-2.89407;,
 -0.00220;-1.22647;-2.89407;,
 -0.00220;-1.22647;-2.89407;,
 -0.00220;-1.22647;-2.89407;,
 -0.00220;-1.22647;-2.89407;,
 -0.00220;-1.45184;-4.80725;,
 -0.00220;-1.22647;-2.89407;,
 1.55658;-1.16835;-6.39105;,
 -0.00220;-1.16835;-6.61724;,
 -0.00220;0.20762;-6.42917;,
 0.96580;-1.46467;-6.14052;,
 -0.00220;-1.46467;-6.26293;,
 -0.00220;-1.16835;-6.61724;,
 -1.56097;-1.16835;-6.39105;,
 -0.00220;0.20762;-6.42917;,
 -0.97019;-1.46467;-6.14052;,
 -0.00220;-1.46467;-6.26293;,
 -0.00220;0.50093;-5.78205;,
 2.38056;0.50093;-4.86802;,
 -2.38496;0.50093;-4.86802;,
 -0.00220;0.50093;-5.78205;,
 2.57752;-0.04969;-5.38092;,
 2.30226;-0.23608;-5.56732;,
 2.57752;-0.04969;-5.64452;,
 2.57752;-0.04969;-5.38092;,
 2.30226;-0.31328;-5.38092;,
 2.57752;-0.04969;-5.38092;,
 2.30226;-0.23608;-5.19453;,
 2.57752;-0.04969;-5.38092;,
 2.57752;-0.04969;-5.11732;,
 2.57752;-0.04969;-5.38092;,
 2.78329;0.13671;-5.19453;,
 2.57752;-0.04969;-5.38092;,
 2.78329;0.21391;-5.38092;,
 2.57752;-0.04969;-5.38092;,
 2.78329;0.13671;-5.56732;,
 2.57752;-0.04969;-5.38092;,
 2.57752;-0.04969;-5.64452;,
 -2.64134;-0.04969;-5.41721;,
 -2.64134;-0.04969;-5.68081;,
 -2.36608;-0.23608;-5.60360;,
 -2.64134;-0.04969;-5.41721;,
 -2.36608;-0.31328;-5.41721;,
 -2.64134;-0.04969;-5.41721;,
 -2.36608;-0.23608;-5.23082;,
 -2.64134;-0.04969;-5.41721;,
 -2.64134;-0.04969;-5.15361;,
 -2.64134;-0.04969;-5.41721;,
 -2.84711;0.13671;-5.23082;,
 -2.64134;-0.04969;-5.41721;,
 -2.84711;0.21391;-5.41721;,
 -2.64134;-0.04969;-5.41721;,
 -2.84711;0.13671;-5.60360;,
 -2.64134;-0.04969;-5.41721;,
 -2.64134;-0.04969;-5.68081;,
 0.00234;-0.04969;-6.68299;,
 0.00287;-0.23608;-6.60578;,
 0.00234;-0.04969;-6.68299;,
 0.00195;0.13671;-6.60578;,
 0.00287;-0.31328;-6.41939;,
 0.00195;0.21391;-6.41939;,
 0.00287;-0.23608;-6.23300;,
 0.00195;0.13671;-6.23300;,
 0.00234;-0.04969;-6.15579;,
 -3.20188;0.20729;-2.89407;,
 3.19713;0.20729;-2.89407;;
 
 60;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 3;14,3,15;,
 3;3,2,16;,
 3;2,5,17;,
 3;5,7,18;,
 3;7,9,19;,
 3;9,11,20;,
 3;11,13,21;,
 3;13,22,23;,
 4;24,25,26,0;,
 4;27,24,0,3;,
 4;28,27,3,14;,
 4;25,24,27,28;,
 4;29,30,12,31;,
 4;32,33,22,13;,
 4;30,32,13,12;,
 4;30,29,33,32;,
 4;34,35,0,26;,
 4;36,37,31,12;,
 3;38,39,40;,
 3;41,42,39;,
 3;43,44,42;,
 3;45,46,44;,
 3;47,48,46;,
 3;49,50,48;,
 3;51,52,50;,
 3;53,54,52;,
 3;55,56,57;,
 3;58,57,59;,
 3;60,59,61;,
 3;62,61,63;,
 3;64,63,65;,
 3;66,65,67;,
 3;68,67,69;,
 3;70,69,71;,
 4;72,40,39,73;,
 4;72,73,57,56;,
 4;74,71,69,75;,
 4;74,75,52,54;,
 4;73,39,42,76;,
 4;73,76,59,57;,
 4;75,69,67,77;,
 4;75,77,50,52;,
 4;76,42,44,78;,
 4;76,78,61,59;,
 4;77,67,65,79;,
 4;77,79,48,50;,
 4;78,44,46,80;,
 4;78,80,63,61;,
 4;79,65,63,80;,
 4;79,80,46,48;,
 4;10,8,6,4;,
 4;1,0,31,12;,
 4;81,10,4,1;,
 4;82,1,12,10;;
 
 MeshMaterialList {
  13;
  60;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  3,
  3,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  3,
  3,
  3,
  3;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Pictures\\Screenshots\\�X�N���[���V���b�g (249).png";
   }
  }
  Material {
   0.228800;0.295200;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.615200;0.615200;0.615200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.050400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.599200;0.599200;0.599200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.009600;0.156800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.080000;0.080000;0.080000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.339200;0.000000;0.060000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.056800;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.392000;0.000000;0.021600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.323200;0.263200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  76;
  -0.000000;0.775864;-0.630900;,
  0.543042;-0.837812;0.056351;,
  0.527070;-0.849093;0.035188;,
  0.389980;-0.849148;0.356180;,
  -0.000000;-0.861074;0.508480;,
  -0.389981;-0.849147;0.356180;,
  -0.527072;-0.849092;0.035189;,
  -0.543043;-0.837812;0.056350;,
  -0.000000;-0.990746;0.135727;,
  0.405714;-0.912383;0.054343;,
  0.399711;-0.916470;0.017697;,
  0.291328;-0.919049;0.265475;,
  -0.000000;-0.925957;0.377629;,
  -0.291329;-0.919049;0.265475;,
  -0.399712;-0.916470;0.017697;,
  -0.405714;-0.912383;0.054343;,
  0.000000;-0.995852;0.090991;,
  -0.000000;0.244666;-0.969607;,
  0.233083;0.237928;-0.942901;,
  -0.233084;0.237927;-0.942901;,
  -0.000000;-0.960620;-0.277864;,
  0.316420;-0.943704;-0.096444;,
  -0.316420;-0.943704;-0.096445;,
  0.000000;0.916946;-0.399011;,
  0.146940;0.906993;-0.394680;,
  -0.146940;0.906993;-0.394680;,
  0.681172;-0.732123;0.000000;,
  0.573786;-0.767628;-0.285512;,
  0.465031;-0.795617;-0.388252;,
  0.382144;-0.924103;0.000006;,
  0.385639;-0.890061;0.243052;,
  0.617558;-0.786526;-0.000000;,
  0.725868;-0.676399;-0.124905;,
  0.788267;-0.615333;-0.000006;,
  0.725868;-0.676399;0.124899;,
  -0.569579;-0.769069;-0.290022;,
  -0.458896;-0.797301;-0.392078;,
  -0.381815;-0.924239;-0.000001;,
  -0.388675;-0.888810;0.242794;,
  -0.617558;-0.786526;0.000000;,
  -0.725868;-0.676399;-0.124905;,
  -0.788267;-0.615333;0.000000;,
  -0.725868;-0.676399;0.124905;,
  -0.681172;-0.732123;0.000000;,
  0.010430;-0.193922;-0.980961;,
  0.007448;-0.777621;-0.628690;,
  0.007983;0.648101;-0.761513;,
  -0.000001;-1.000000;0.000001;,
  -0.000001;1.000000;0.000001;,
  -0.005319;-0.923875;0.382657;,
  -0.006485;0.761489;0.648145;,
  -0.010470;0.215646;0.976416;,
  0.000000;0.707107;-0.707107;,
  -0.243374;0.768743;0.591442;,
  0.148667;0.839101;-0.523266;,
  0.562075;-0.790915;0.241920;,
  -0.000000;-0.758739;-0.651394;,
  0.088190;-0.755783;-0.648857;,
  -0.148667;0.839101;-0.523267;,
  -0.562074;-0.790916;0.241917;,
  -0.088191;-0.755783;-0.648856;,
  -0.336173;0.195406;-0.921306;,
  -0.264730;0.626363;-0.733204;,
  0.280766;0.620681;-0.732069;,
  0.355354;0.183631;-0.916517;,
  -0.000000;1.000000;-0.000001;,
  -0.000002;1.000000;0.000003;,
  0.228817;0.740024;0.632461;,
  -0.241193;0.740297;0.627524;,
  -0.011528;-0.114292;0.993380;,
  -0.390508;-0.097576;0.915414;,
  -0.370561;0.205917;0.905694;,
  0.351778;0.196242;0.915282;,
  0.369181;-0.113867;0.922356;,
  0.000000;0.923880;-0.382683;,
  0.000000;1.000000;0.000000;;
  60;
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  3;8,9,16;,
  3;9,10,16;,
  3;10,11,16;,
  3;11,12,16;,
  3;12,13,16;,
  3;13,14,16;,
  3;14,15,16;,
  3;15,8,16;,
  4;18,17,0,54;,
  4;21,55,1,9;,
  4;20,21,9,8;,
  4;56,57,21,20;,
  4;17,19,58,0;,
  4;22,20,8,15;,
  4;59,22,15,7;,
  4;60,56,20,22;,
  4;23,24,54,0;,
  4;25,23,0,58;,
  3;26,28,27;,
  3;26,29,28;,
  3;26,30,29;,
  3;26,31,30;,
  3;26,32,31;,
  3;26,33,32;,
  3;26,34,33;,
  3;26,27,34;,
  3;43,35,36;,
  3;43,36,37;,
  3;43,37,38;,
  3;43,38,39;,
  3;43,39,40;,
  3;43,40,41;,
  3;43,41,42;,
  3;43,42,35;,
  4;44,27,28,45;,
  4;44,45,36,35;,
  4;44,61,62,46;,
  4;44,46,63,64;,
  4;45,28,29,47;,
  4;45,47,37,36;,
  4;46,62,65,48;,
  4;46,48,66,63;,
  4;47,29,30,49;,
  4;47,49,38,37;,
  4;48,65,67,50;,
  4;48,50,68,66;,
  4;69,70,71,51;,
  4;69,51,72,73;,
  4;50,67,72,51;,
  4;50,51,71,68;,
  4;74,75,75,74;,
  4;74,54,0,58;,
  4;52,74,74,74;,
  4;53,53,53,53;;
 }
 MeshTextureCoords {
  83;
  0.125000;0.750000;,
  0.250000;0.750000;,
  0.250000;0.875000;,
  0.125000;0.875000;,
  0.375000;0.750000;,
  0.375000;0.875000;,
  0.500000;0.750000;,
  0.500000;0.875000;,
  0.625000;0.750000;,
  0.625000;0.875000;,
  0.750000;0.750000;,
  0.750000;0.875000;,
  0.875000;0.750000;,
  0.875000;0.875000;,
  0.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  1.000000;0.875000;,
  0.937500;1.000000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  1.000000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  1.000000;0.500000;,
  0.875000;0.500000;,
  0.250000;0.500000;,
  0.750000;0.500000;,
  0.375000;0.500000;,
  0.625000;0.500000;,
  0.500000;0.500000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}