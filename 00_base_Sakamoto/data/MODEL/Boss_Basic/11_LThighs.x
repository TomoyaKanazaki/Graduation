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
 66;
 -4.71754;-13.89532;2.09926;,
 -3.49945;-15.72379;2.55740;,
 -3.26155;-15.72379;6.05145;,
 -3.87293;-13.89532;6.00342;,
 0.00000;0.97061;0.00000;,
 -4.19177;0.97062;1.93992;,
 -3.44129;0.97062;5.87099;,
 0.00000;0.97062;6.62049;,
 4.19176;0.97062;1.93992;,
 3.44128;0.97062;5.87099;,
 3.49944;-15.72379;2.55740;,
 4.71753;-13.89532;2.09926;,
 3.87291;-13.89532;6.00342;,
 2.87291;-15.72379;6.05145;,
 -0.00000;-16.40754;0.88917;,
 2.59646;-16.40754;2.19280;,
 2.13160;-16.40754;4.78527;,
 -0.00000;-16.40754;5.27955;,
 -2.59648;-16.40754;2.19280;,
 -2.41996;-16.40754;4.78527;,
 0.00000;-15.72379;6.71763;,
 0.00000;-13.89532;6.74779;,
 3.26154;-15.72379;-2.54858;,
 0.00000;-15.72379;-4.42881;,
 0.00000;-13.89532;-5.70691;,
 4.39682;-13.89532;-3.60600;,
 -3.26155;-15.72379;-2.54858;,
 -4.39683;-13.89532;-3.60600;,
 -0.00000;-16.40754;-2.99074;,
 -2.41996;-16.40754;-1.59567;,
 2.41995;-16.40754;-1.59567;,
 0.00000;0.97062;-5.92010;,
 3.90680;0.97062;-3.80470;,
 -3.90680;0.97062;-3.80470;,
 2.48622;-16.11190;5.39095;,
 3.02841;-16.11190;2.36721;,
 -0.00000;-16.11190;5.96746;,
 -2.82254;-16.11190;5.39095;,
 -3.02842;-16.11190;2.36721;,
 -2.82254;-16.11190;-2.05150;,
 -0.00000;-16.11190;-3.67864;,
 2.82253;-16.11190;-2.05150;,
 -4.85689;-4.55770;6.43035;,
 -5.91608;-4.55770;1.53138;,
 0.00000;-4.55770;7.36439;,
 4.85687;-4.55770;6.43035;,
 5.91607;-4.55770;1.53138;,
 5.51388;-4.55770;-5.62766;,
 0.00000;-4.55770;-8.26391;,
 -5.51389;-4.55770;-5.62766;,
 -5.06660;-1.10567;7.15406;,
 -6.17152;-1.10567;2.05963;,
 0.00000;-1.10567;8.12537;,
 5.06657;-1.10567;7.15406;,
 6.17151;-1.10567;2.05963;,
 5.75196;-1.10567;-5.38505;,
 0.00000;-1.10567;-8.12648;,
 -5.75197;-1.10567;-5.38505;,
 -4.51400;0.00000;6.71125;,
 -5.49842;0.00000;2.17245;,
 0.00000;0.00000;7.57663;,
 4.51399;0.00000;6.71125;,
 5.49841;0.00000;2.17245;,
 5.12461;0.00000;-4.46026;,
 0.00000;0.00000;-6.90270;,
 -5.12462;0.00000;-4.46026;;
 
 64;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,4,7,9;,
 4;10,11,12,13;,
 4;14,15,16,17;,
 4;18,14,17,19;,
 4;20,13,12,21;,
 4;2,20,21,3;,
 4;22,23,24,25;,
 4;23,26,27,24;,
 4;28,14,18,29;,
 4;30,15,14,28;,
 4;25,11,10,22;,
 4;31,4,8,32;,
 4;33,5,4,31;,
 4;26,1,0,27;,
 4;10,13,34,35;,
 4;13,20,36,34;,
 4;20,2,37,36;,
 4;2,1,38,37;,
 4;1,26,39,38;,
 4;26,23,40,39;,
 4;22,10,35,41;,
 4;23,22,41,40;,
 4;35,34,16,15;,
 4;34,36,17,16;,
 4;36,37,19,17;,
 4;37,38,18,19;,
 4;38,39,29,18;,
 4;39,40,28,29;,
 4;41,35,15,30;,
 4;40,41,30,28;,
 4;0,3,42,43;,
 4;3,21,44,42;,
 4;21,12,45,44;,
 4;12,11,46,45;,
 4;11,25,47,46;,
 4;25,24,48,47;,
 4;27,0,43,49;,
 4;24,27,49,48;,
 4;43,42,50,51;,
 4;42,44,52,50;,
 4;44,45,53,52;,
 4;45,46,54,53;,
 4;46,47,55,54;,
 4;47,48,56,55;,
 4;49,43,51,57;,
 4;48,49,57,56;,
 4;51,50,58,59;,
 4;50,52,60,58;,
 4;52,53,61,60;,
 4;53,54,62,61;,
 4;54,55,63,62;,
 4;55,56,64,63;,
 4;57,51,59,65;,
 4;56,57,65,64;,
 4;59,58,6,5;,
 4;58,60,7,6;,
 4;60,61,9,7;,
 4;61,62,8,9;,
 4;62,63,32,8;,
 4;63,64,31,32;,
 4;65,59,5,33;,
 4;64,65,33,31;;
 
 MeshMaterialList {
  8;
  64;
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
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  79;
  -0.571694;-0.748780;-0.335401;,
  -0.597418;-0.768143;0.230320;,
  -0.755868;-0.435153;-0.489189;,
  -0.948350;-0.268291;0.169270;,
  0.000001;-0.458336;-0.888779;,
  0.003260;-0.051684;0.998658;,
  0.755869;-0.435148;-0.489191;,
  0.931437;-0.308340;0.193266;,
  0.571695;-0.748780;-0.335401;,
  0.565227;-0.781189;0.265070;,
  0.000000;-0.778080;-0.628165;,
  0.006527;-0.037456;0.999277;,
  -0.765965;-0.642435;0.023954;,
  0.747473;-0.662245;0.052106;,
  0.940434;-0.332204;0.072286;,
  -0.948290;-0.311586;0.060504;,
  0.590943;-0.805858;0.037131;,
  0.349804;-0.895339;0.275690;,
  0.005853;-0.904034;0.427421;,
  -0.599414;-0.800417;0.005930;,
  -0.357335;-0.900998;0.245997;,
  -0.000000;-0.904105;-0.427310;,
  -0.439947;-0.868012;-0.230220;,
  0.439947;-0.868012;-0.230219;,
  0.000000;-1.000000;-0.000000;,
  0.289634;-0.956966;0.018145;,
  0.218625;-0.960768;0.170670;,
  0.002759;-0.979550;0.201181;,
  -0.294644;-0.955603;0.002898;,
  -0.222390;-0.963163;0.151192;,
  -0.000000;-0.979997;-0.199010;,
  -0.277652;-0.949832;-0.143972;,
  0.277652;-0.949832;-0.143972;,
  -0.991549;-0.103711;0.077938;,
  -0.971680;-0.107797;0.210281;,
  0.000000;-0.141207;0.989980;,
  0.991549;-0.103712;0.077938;,
  0.186860;-0.138722;0.972543;,
  0.000001;-0.114535;-0.993419;,
  0.993640;-0.097761;-0.055875;,
  -0.993640;-0.097762;-0.055874;,
  -0.971191;0.225555;0.076898;,
  -0.956179;0.206848;0.207211;,
  0.000000;0.121828;0.992551;,
  0.971191;0.225555;0.076898;,
  0.187075;0.119673;0.975029;,
  0.000000;0.433773;-0.901022;,
  0.969167;0.240284;-0.054573;,
  -0.969167;0.240284;-0.054573;,
  -0.741615;0.668350;0.057579;,
  -0.494380;0.689110;0.529826;,
  0.000000;0.589865;0.807502;,
  0.741615;0.668350;0.057580;,
  0.494380;0.689110;0.529826;,
  0.000000;0.746335;-0.665570;,
  0.563678;0.741316;-0.364305;,
  -0.563678;0.741316;-0.364305;,
  -0.000000;1.000000;-0.000000;,
  -0.316563;0.948280;0.023514;,
  -0.268657;0.916489;0.296432;,
  0.000000;0.927480;0.373873;,
  0.316563;0.948280;0.023515;,
  0.268657;0.916488;0.296433;,
  0.000000;0.939132;-0.343557;,
  0.336238;0.910901;-0.239174;,
  -0.336237;0.910901;-0.239174;,
  0.207155;-0.046660;0.977195;,
  0.197796;-0.055676;0.978661;,
  -0.194368;-0.026723;0.980565;,
  -0.191421;-0.045714;0.980443;,
  0.006285;-0.888153;0.459505;,
  -0.186859;-0.138717;0.972544;,
  0.971679;-0.107800;0.210282;,
  0.428701;-0.103472;-0.897502;,
  -0.428700;-0.103480;-0.897501;,
  -0.187075;0.119681;0.975028;,
  0.956178;0.206848;0.207212;,
  0.394897;0.398518;-0.827792;,
  -0.394897;0.398518;-0.827792;;
  64;
  4;15,12,1,3;,
  4;57,58,59,60;,
  4;61,57,60,62;,
  4;13,14,7,9;,
  4;24,25,26,27;,
  4;28,24,27,29;,
  4;11,66,67,5;,
  4;68,11,5,69;,
  4;8,10,4,6;,
  4;10,0,2,4;,
  4;30,24,28,31;,
  4;32,25,24,30;,
  4;6,14,13,8;,
  4;63,57,61,64;,
  4;65,58,57,63;,
  4;0,12,15,2;,
  4;13,9,17,16;,
  4;9,70,18,17;,
  4;70,1,20,18;,
  4;1,12,19,20;,
  4;12,0,22,19;,
  4;0,10,21,22;,
  4;8,13,16,23;,
  4;10,8,23,21;,
  4;16,17,26,25;,
  4;17,18,27,26;,
  4;18,20,29,27;,
  4;20,19,28,29;,
  4;19,22,31,28;,
  4;22,21,30,31;,
  4;23,16,25,32;,
  4;21,23,32,30;,
  4;15,3,34,33;,
  4;69,5,35,71;,
  4;5,67,37,35;,
  4;7,14,36,72;,
  4;14,6,39,36;,
  4;6,4,38,73;,
  4;2,15,33,40;,
  4;4,2,74,38;,
  4;33,34,42,41;,
  4;71,35,43,75;,
  4;35,37,45,43;,
  4;72,36,44,76;,
  4;36,39,47,44;,
  4;73,38,46,77;,
  4;40,33,41,48;,
  4;38,74,78,46;,
  4;41,42,50,49;,
  4;75,43,51,50;,
  4;43,45,53,51;,
  4;76,44,52,53;,
  4;44,47,55,52;,
  4;77,46,54,55;,
  4;48,41,49,56;,
  4;46,78,56,54;,
  4;49,50,59,58;,
  4;50,51,60,59;,
  4;51,53,62,60;,
  4;53,52,61,62;,
  4;52,55,64,61;,
  4;55,54,63,64;,
  4;56,49,58,65;,
  4;54,56,65,63;;
 }
 MeshTextureCoords {
  66;
  0.862870;0.577430;,
  0.841380;0.600350;,
  0.837180;0.600350;,
  0.847970;0.577430;,
  0.779610;0.391080;,
  0.853600;0.391080;,
  0.840350;0.391080;,
  0.779610;0.391080;,
  0.705630;0.391080;,
  0.718880;0.391080;,
  0.717850;0.600350;,
  0.696350;0.577430;,
  0.711260;0.577430;,
  0.728910;0.600350;,
  0.779610;0.608920;,
  0.733790;0.608920;,
  0.741990;0.608920;,
  0.779610;0.608920;,
  0.825440;0.608920;,
  0.822320;0.608920;,
  0.779610;0.600350;,
  0.779610;0.577430;,
  0.722050;0.600350;,
  0.779610;0.600350;,
  0.779610;0.577430;,
  0.702010;0.577430;,
  0.837180;0.600350;,
  0.857210;0.577430;,
  0.779610;0.608920;,
  0.822320;0.608920;,
  0.736900;0.608920;,
  0.779610;0.391080;,
  0.710660;0.391080;,
  0.848560;0.391080;,
  0.735730;0.605220;,
  0.726160;0.605220;,
  0.779610;0.605220;,
  0.829430;0.605220;,
  0.833060;0.605220;,
  0.829430;0.605220;,
  0.779610;0.605220;,
  0.729800;0.605220;,
  0.865330;0.460380;,
  0.884030;0.460380;,
  0.779610;0.460380;,
  0.693890;0.460380;,
  0.675200;0.460380;,
  0.682300;0.460380;,
  0.779610;0.460380;,
  0.876930;0.460380;,
  0.869040;0.417100;,
  0.888540;0.417100;,
  0.779610;0.417100;,
  0.690190;0.417100;,
  0.670690;0.417100;,
  0.678090;0.417100;,
  0.779610;0.417100;,
  0.881130;0.417100;,
  0.859280;0.403240;,
  0.876660;0.403240;,
  0.779610;0.403240;,
  0.699940;0.403240;,
  0.682570;0.403240;,
  0.689170;0.403240;,
  0.779610;0.403240;,
  0.870060;0.403240;;
 }
}