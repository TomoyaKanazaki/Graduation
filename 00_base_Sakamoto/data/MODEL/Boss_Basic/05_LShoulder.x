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
 74;
 -1.19865;-3.74630;-4.20874;,
 -1.11099;-4.72860;0.00000;,
 -2.44592;0.00000;0.00000;,
 -1.19865;0.00000;-4.34354;,
 -1.34185;-3.74630;3.27965;,
 -1.34185;0.00000;2.91701;,
 -1.61873;3.88952;-0.10171;,
 -1.70638;3.31444;-3.94849;,
 -1.84959;3.31444;2.48169;,
 0.78825;5.34977;-6.04962;,
 0.92982;6.27853;0.16289;,
 7.52435;6.03209;0.06284;,
 7.52435;4.62982;-5.25517;,
 0.55697;5.34977;4.33504;,
 7.52435;4.62982;3.63429;,
 12.33651;5.50541;0.06284;,
 12.33651;4.21884;-5.25517;,
 12.33651;4.21884;3.63429;,
 17.43947;4.86464;0.06284;,
 17.43947;3.71884;-5.25517;,
 17.43947;3.71884;3.63429;,
 19.43324;2.33610;-3.73587;,
 19.43324;3.09266;-0.22435;,
 19.43324;-0.18736;-0.13151;,
 19.43324;-0.18736;-4.78250;,
 19.43324;2.33610;2.13390;,
 19.43324;-0.18736;3.21727;,
 19.43324;-3.79616;-0.13151;,
 19.43324;-3.03956;-3.97346;,
 19.43324;-3.03956;2.86231;,
 17.43947;-4.42230;-5.61497;,
 17.43947;-5.56811;0.20345;,
 12.33651;-6.20890;0.20345;,
 12.33651;-4.92231;-5.61497;,
 17.43947;-4.42230;4.73742;,
 12.33651;-4.92231;4.73742;,
 7.52435;-6.73556;0.20345;,
 7.52435;-5.33329;-5.61497;,
 7.52435;-5.33329;4.73742;,
 1.74981;-7.63966;0.32715;,
 1.60824;-6.05322;-6.46993;,
 1.37697;-6.05322;5.62373;,
 17.43947;-0.10277;5.27500;,
 12.33651;-0.07222;5.27500;,
 7.52435;-0.04704;5.27500;,
 1.37697;-0.00301;5.03808;,
 12.33651;-0.07222;-6.84022;,
 17.43947;-0.10277;-6.84022;,
 7.52435;-0.04704;-6.84022;,
 1.60824;-0.00301;-6.68763;,
 1.74981;-7.63966;0.32715;,
 1.60824;-6.05322;-6.46993;,
 1.60824;-0.00301;-6.68763;,
 1.37697;-6.05322;5.62373;,
 1.37697;-0.00301;5.03808;,
 0.78825;5.34977;-6.04962;,
 0.92982;6.27853;0.16289;,
 0.55697;5.34977;4.33504;,
 0.11164;-5.90767;0.14555;,
 -0.00000;-4.65657;-5.21480;,
 0.00000;0.11480;-5.38648;,
 -0.18239;-4.65657;4.32258;,
 -0.18239;0.11480;3.86073;,
 -0.64667;4.33615;-4.88333;,
 -0.53502;5.06859;0.01602;,
 -0.82906;4.33615;3.30629;,
 18.53700;4.12922;-0.05636;,
 18.53700;3.14495;-4.62461;,
 18.53700;-0.13789;-5.98619;,
 18.53700;3.14495;3.01157;,
 18.53700;-0.13789;4.42096;,
 18.53700;-3.84842;-4.93368;,
 18.53700;-4.83269;0.06443;,
 18.53700;-3.84842;3.95918;;
 
 72;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;3,2,6,7;,
 4;2,5,8,6;,
 4;9,10,11,12;,
 4;10,13,14,11;,
 4;12,11,15,16;,
 4;11,14,17,15;,
 4;16,15,18,19;,
 4;15,17,20,18;,
 4;21,22,23,24;,
 4;22,25,26,23;,
 4;24,23,27,28;,
 4;23,26,29,27;,
 4;30,31,32,33;,
 4;31,34,35,32;,
 4;33,32,36,37;,
 4;32,35,38,36;,
 4;37,36,39,40;,
 4;36,38,41,39;,
 4;35,34,42,43;,
 4;38,35,43,44;,
 4;41,38,44,45;,
 4;43,42,20,17;,
 4;44,43,17,14;,
 4;45,44,14,13;,
 4;30,33,46,47;,
 4;33,37,48,46;,
 4;37,40,49,48;,
 4;47,46,16,19;,
 4;46,48,12,16;,
 4;48,49,9,12;,
 4;40,39,50,51;,
 4;49,40,51,52;,
 4;39,41,53,50;,
 4;41,45,54,53;,
 4;10,9,55,56;,
 4;9,49,52,55;,
 4;45,13,57,54;,
 4;13,10,56,57;,
 4;51,50,58,59;,
 4;52,51,59,60;,
 4;50,53,61,58;,
 4;53,54,62,61;,
 4;56,55,63,64;,
 4;55,52,60,63;,
 4;54,57,65,62;,
 4;57,56,64,65;,
 4;59,58,1,0;,
 4;60,59,0,3;,
 4;58,61,4,1;,
 4;61,62,5,4;,
 4;64,63,7,6;,
 4;63,60,3,7;,
 4;62,65,8,5;,
 4;65,64,6,8;,
 4;19,18,66,67;,
 4;47,19,67,68;,
 4;18,20,69,66;,
 4;20,42,70,69;,
 4;31,30,71,72;,
 4;30,47,68,71;,
 4;42,34,73,70;,
 4;34,31,72,73;,
 4;67,66,22,21;,
 4;68,67,21,24;,
 4;66,69,25,22;,
 4;69,70,26,25;,
 4;72,71,28,27;,
 4;71,68,24,28;,
 4;70,73,29,26;,
 4;73,72,27,29;;
 
 MeshMaterialList {
  8;
  72;
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7;;
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
  87;
  0.147803;-0.961963;-0.229741;,
  0.156043;-0.987212;0.032602;,
  0.153433;-0.943821;0.292679;,
  0.058627;0.050289;-0.997012;,
  0.043681;0.119702;0.991849;,
  0.077560;0.976176;-0.202647;,
  0.082254;0.995429;0.048540;,
  0.069985;0.683659;0.726439;,
  0.069769;0.726094;-0.684047;,
  0.089457;0.994621;0.052213;,
  0.063986;0.693188;0.717911;,
  0.066577;0.745941;-0.662676;,
  0.103840;0.993146;0.053641;,
  0.063168;0.713474;0.697828;,
  0.361493;0.713667;-0.600001;,
  0.346615;0.936862;0.046342;,
  0.358566;0.685460;0.633699;,
  0.334454;0.053937;-0.940867;,
  0.330905;0.130072;0.934657;,
  0.470764;-0.768482;-0.433379;,
  0.345719;-0.938016;0.024596;,
  0.345309;-0.908040;0.237119;,
  0.101596;-0.971594;-0.213739;,
  0.103951;-0.994155;0.029139;,
  0.100121;-0.957559;0.270290;,
  0.121176;-0.966326;-0.227002;,
  0.126483;-0.991467;0.031540;,
  0.123251;-0.949671;0.287982;,
  0.011017;0.128035;0.991708;,
  0.026906;0.121053;0.992281;,
  0.013862;0.053517;-0.998471;,
  0.035777;0.050808;-0.998067;,
  0.000000;0.000000;0.000000;,
  -0.751730;-0.411665;-0.515203;,
  -0.710697;-0.703493;0.002807;,
  -0.642522;-0.017334;-0.766071;,
  -0.772651;-0.487774;0.406308;,
  -0.616595;0.042422;0.786137;,
  -0.688347;0.725377;0.002357;,
  -0.760142;0.543988;-0.355332;,
  -0.739588;0.257972;0.621659;,
  -0.856043;-0.330341;-0.397573;,
  -0.891700;-0.452537;0.008994;,
  -0.999115;-0.040642;0.010813;,
  -0.873885;-0.035327;-0.484848;,
  -0.863304;-0.280440;0.419594;,
  -0.862254;0.003926;0.506460;,
  -0.921139;0.389215;0.003806;,
  -0.889757;0.268604;-0.369031;,
  -0.882532;0.274335;0.381940;,
  0.711491;0.547063;-0.441024;,
  0.665189;0.745892;0.034191;,
  0.715527;0.041445;-0.697355;,
  0.711322;0.523899;0.468563;,
  0.713505;0.100782;0.693364;,
  0.664248;-0.747312;0.017302;,
  0.726961;-0.514600;-0.454659;,
  0.755415;-0.443043;0.482764;,
  0.909954;0.324312;-0.258469;,
  0.938637;0.344563;0.015393;,
  1.000000;0.000000;0.000000;,
  0.950376;0.018555;-0.310550;,
  0.909371;0.310821;0.276467;,
  0.949568;0.045447;0.310250;,
  0.938167;-0.346101;0.007601;,
  0.916557;-0.301546;-0.262665;,
  0.927413;-0.254604;0.274010;,
  0.004530;-0.111251;0.993782;,
  0.324090;-0.113317;0.939215;,
  0.025263;-0.054324;0.998204;,
  0.046417;-0.002812;0.998918;,
  0.011181;-0.258921;-0.965834;,
  0.010092;-0.247161;-0.968922;,
  0.034909;-0.183869;-0.982331;,
  0.060698;-0.131742;-0.989424;,
  0.054609;0.230650;-0.971503;,
  -0.754333;-0.402616;-0.518538;,
  -0.726618;-0.687038;0.002099;,
  -0.629597;-0.015829;-0.776761;,
  -0.726101;-0.671054;0.149882;,
  -0.597035;0.077292;0.798483;,
  -0.599786;0.045002;0.798894;,
  -0.637156;0.770715;0.005558;,
  -0.627524;0.772003;-0.101114;,
  -0.629135;-0.003729;-0.777287;,
  -0.601906;0.012665;0.798467;,
  -0.639476;0.760585;0.112166;;
  72;
  4;41,42,43,44;,
  4;42,45,46,43;,
  4;44,43,47,48;,
  4;43,46,49,47;,
  4;5,6,9,8;,
  4;6,7,10,9;,
  4;8,9,12,11;,
  4;9,10,13,12;,
  4;11,12,15,14;,
  4;12,13,16,15;,
  4;58,59,60,61;,
  4;59,62,63,60;,
  4;61,60,64,65;,
  4;60,63,66,64;,
  4;19,20,23,22;,
  4;20,21,24,23;,
  4;22,23,26,25;,
  4;23,24,27,26;,
  4;25,26,1,0;,
  4;26,27,2,1;,
  4;67,68,18,28;,
  4;69,67,28,29;,
  4;70,69,29,4;,
  4;28,18,16,13;,
  4;29,28,13,10;,
  4;4,29,10,7;,
  4;71,72,30,17;,
  4;72,73,31,30;,
  4;73,74,3,31;,
  4;17,30,11,14;,
  4;30,31,8,11;,
  4;31,3,75,8;,
  4;32,32,32,32;,
  4;32,32,32,32;,
  4;32,32,32,32;,
  4;32,32,32,32;,
  4;32,32,32,32;,
  4;32,32,32,32;,
  4;32,32,32,32;,
  4;32,32,32,32;,
  4;76,77,34,33;,
  4;78,76,33,35;,
  4;77,79,36,34;,
  4;80,81,37,80;,
  4;82,83,39,38;,
  4;84,78,35,84;,
  4;81,85,40,37;,
  4;86,82,38,86;,
  4;33,34,42,41;,
  4;35,33,41,44;,
  4;34,36,45,42;,
  4;36,37,46,45;,
  4;38,39,48,47;,
  4;39,35,44,48;,
  4;37,40,49,46;,
  4;40,38,47,49;,
  4;14,15,51,50;,
  4;17,14,50,52;,
  4;15,16,53,51;,
  4;16,18,54,53;,
  4;20,19,56,55;,
  4;19,17,52,56;,
  4;18,68,57,54;,
  4;21,20,55,57;,
  4;50,51,59,58;,
  4;52,50,58,61;,
  4;51,53,62,59;,
  4;53,54,63,62;,
  4;55,56,65,64;,
  4;56,52,61,65;,
  4;54,57,66,63;,
  4;57,55,64,66;;
 }
 MeshTextureCoords {
  74;
  0.884050;0.555360;,
  0.883040;0.573100;,
  0.898380;0.487710;,
  0.884050;0.487710;,
  0.885700;0.555360;,
  0.885700;0.487710;,
  0.888880;0.417470;,
  0.889880;0.427850;,
  0.891530;0.427850;,
  0.861220;0.391090;,
  0.859600;0.374320;,
  0.783840;0.378770;,
  0.783840;0.404100;,
  0.863880;0.391090;,
  0.783840;0.404100;,
  0.728550;0.388280;,
  0.728550;0.411520;,
  0.728550;0.411520;,
  0.669930;0.399860;,
  0.669930;0.420550;,
  0.669930;0.420550;,
  0.647020;0.445520;,
  0.647020;0.431860;,
  0.647020;0.491090;,
  0.647020;0.491090;,
  0.647020;0.445520;,
  0.647020;0.491090;,
  0.647020;0.556270;,
  0.647020;0.542600;,
  0.647020;0.542600;,
  0.669930;0.567570;,
  0.669930;0.588270;,
  0.728550;0.599840;,
  0.728550;0.576600;,
  0.669930;0.567570;,
  0.728550;0.576600;,
  0.783840;0.609350;,
  0.783840;0.584030;,
  0.783840;0.584030;,
  0.850180;0.625680;,
  0.851800;0.597030;,
  0.854460;0.597030;,
  0.669930;0.489570;,
  0.728550;0.489010;,
  0.783840;0.488560;,
  0.854460;0.487760;,
  0.728550;0.489010;,
  0.669930;0.489570;,
  0.783840;0.488560;,
  0.851800;0.487760;,
  0.850180;0.625680;,
  0.851800;0.597030;,
  0.851800;0.487760;,
  0.854460;0.597030;,
  0.854460;0.487760;,
  0.861220;0.391090;,
  0.859600;0.374320;,
  0.863880;0.391090;,
  0.869000;0.594400;,
  0.870280;0.571810;,
  0.870280;0.485640;,
  0.872380;0.571810;,
  0.872380;0.485640;,
  0.877710;0.409400;,
  0.876430;0.396170;,
  0.879800;0.409400;,
  0.657320;0.413140;,
  0.657320;0.430910;,
  0.657320;0.490200;,
  0.657320;0.430910;,
  0.657320;0.490200;,
  0.657320;0.557210;,
  0.657320;0.574990;,
  0.657320;0.557210;;
 }
}