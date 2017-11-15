#version 330
//for large data
in vec3 EntryPoint;
in vec4 ExitPointCoord;
in mat4 mvpMat;

uniform sampler2D MeshColor;
uniform sampler2D MeshDepth;
uniform sampler2D MeshColorBack;
uniform sampler2D MeshDepthBack;
uniform sampler2D EntryPoints;
uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;  
uniform float     StepSize;
uniform vec3      DimensionSize;

//uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

layout (location = 0) out vec4 FragColor;

void main()
{ 	vec2 exitFragCoord = (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0;
	vec3 EntryCoord = texture(EntryPoints, exitFragCoord).xyz;
   
    vec3 exitPoint  = texture(ExitPoints, exitFragCoord).xyz;
	
    if (EntryCoord == exitPoint)
    	//background need no raycasting
    	discard;
    vec3 dir = exitPoint - EntryCoord;
    float len = length(dir); // the length from front to back is calculated and used to terminate the ray
    vec3 deltaDir = normalize(dir) * StepSize;
    float deltaDirLen = length(deltaDir);
    vec3 voxelCoord = EntryCoord;
    vec4 colorAcum = vec4(0.0); // The dest color
    float alphaAcum = 0.0;                // The  dest alpha for blending
    /* ¶¨ÒåÑÕÉ«²éÕÒµÄ×ø±ê */
    float intensity;
    float lengthAcum = 0.0;
    vec4 colorSample; // The src color 
    float alphaSample; // The src alpha
    // backgroundColor
    vec4 bgColor = vec4(0.0, 0.0, 0.0, 1.0);

	int frontFlag = 0, backFlag = 0;
    for(int i = 0; i < 1600; i++)
    {
		
		float intensity = texture(VolumeTex, voxelCoord).y;
        //calc norm here
        vec4 xyzw;
        if(voxelCoord.x==0)
            xyzw.y=texture(VolumeTex,vec3(voxelCoord.x+0.004,voxelCoord.y,voxelCoord.z)).x-intensity;
        else if(voxelCoord.x==DimensionSize.x)
            xyzw.y=intensity-texture(VolumeTex,vec3(voxelCoord.x-0.004,voxelCoord.y,voxelCoord.z)).x;
        else
            xyzw.y=(texture(VolumeTex,vec3(voxelCoord.x+0.004,voxelCoord.y,voxelCoord.z)).x-texture(VolumeTex,vec3(voxelCoord.x-0.004,voxelCoord.y,voxelCoord.z))).x/2.0;
        if(voxelCoord.y==0)
            xyzw.z=texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y+0.004,voxelCoord.z)).x-intensity;
        else if(voxelCoord.y==DimensionSize.y)
            xyzw.z=intensity-texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y-0.004,voxelCoord.z)).x;
        else
            xyzw.z=(texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y+0.004,voxelCoord.z)).x-texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y-0.004,voxelCoord.z))).x/2.0;
        if(voxelCoord.z==0)
            xyzw.w=texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y,voxelCoord.z+0.004)).x-intensity;
        else if(voxelCoord.z==DimensionSize.z)
            xyzw.w=intensity-texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y,voxelCoord.z-0.004)).x;
        else
            xyzw.w=(texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y,voxelCoord.z+0.004)).x-texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y,voxelCoord.z-0.004))).x/2.0;


		vec3 norm =  normalize(xyzw.yzw);
		vec4 currentCoord = mvpMat * vec4(voxelCoord, 1);

		vec3 lightDir = normalize(lightPos - voxelCoord);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;
		float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * lightColor;

		float specularStrength = 0.5f;
		vec3 viewDir = normalize(viewPos - voxelCoord);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
		vec3 specular = specularStrength * spec * lightColor;

    	// ²éÕÒ´«Êäº¯ÊýÖÐÓ³ÉäºóµÄÖµ
    	// ÒÀÀµÐÔÎÆÀí¶ÁÈ¡
    	colorSample = texture(TransferFunc, intensity);

		colorSample.rgb = (ambient + diffuse + specular) * colorSample.rgb;
		
    	// modulate the value of colorSample.a
    	// front-to-back integration
    	if (colorSample.a > 0.0) {
    	    // accomodate for variable sampling rates (base interval defined by mod_compositing.frag)
    	    colorSample.a = 1.0 - pow(1.0 - colorSample.a, StepSize*200.0f);
    	    colorAcum.rgb += (1.0 - colorAcum.a) * colorSample.rgb * colorSample.a;
    	    colorAcum.a += (1.0 - colorAcum.a) * colorSample.a;
    	}
    	voxelCoord += deltaDir;
    	lengthAcum += deltaDirLen;
    	if (lengthAcum >= len)
    	{	
    	    colorAcum.rgb = colorAcum.rgb*colorAcum.a + (1 - colorAcum.a) * bgColor.rgb;		
    	    break;  // terminate if opacity > 1 or the ray is outside the volume	
    	} else if (colorAcum.a > 1.0) {
    	    colorAcum.a = 1.0;
    	    break;
    	}

		float frontDepth = texture(MeshDepth, (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0).x;
		vec3 frontColor = texture(MeshColor, (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0).xyz;
		if (frontFlag == 0 && (currentCoord.z / currentCoord.w + 1.0) / 2 >= frontDepth) {
			frontFlag = 1;
			colorSample.a = 1.0 - pow(1.0 - colorSample.a - 0.5, StepSize*200.0f);
			colorAcum.rgb = colorAcum.rgb * colorAcum.a + (1 - colorAcum.a) * frontColor * 0.5;
			colorAcum.a += (1.0 - colorAcum.a) * 0.5;
		}

		float backDepth = texture(MeshDepthBack, (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0).x;
		vec3 backColor = texture(MeshColorBack, (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0).xyz;
		if (backFlag == 0 && (currentCoord.z / currentCoord.w + 1.0) / 2 >= backDepth) {
			backFlag = 1;
			colorSample.a = 1.0 - pow(1.0 - colorSample.a - 0.5, StepSize*200.0f);
			colorAcum.rgb = colorAcum.rgb * colorAcum.a + (1 - colorAcum.a) * backColor * 0.5;
			colorAcum.a += (1.0 - colorAcum.a) * 0.5;
		}
		
		//(voxelCoord - gl_FragCoord).xy;
		/*if (currentCoord.z > texture(MeshDepth, (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0).x) {
			// terminate if z depth is larger than mesh depth
		    colorAcum.rgb = colorAcum.rgb * colorAcum.a + (1 - colorAcum.a) * texture(MeshColor, (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0).xyz;
			break;
		}*/
		/*if (EntryCoord.z == gl_FragCoord.z) {
			colorAcum.rgb = vec3(1, 0, 0);
		}*/
    }
	
	colorAcum.rgb = colorAcum.rgb;

	// FragColor = colorAcum;
	/*if (texture(MeshDepthBack, gl_FragCoord.st/ScreenSize).x < 1) {
	    // FragColor = vec4(length(voxelCoord - viewPos), 0, 0, 1);
		FragColor = vec4(texture(MeshColorBack, gl_FragCoord.st/ScreenSize).xyz, 1);
		// FragColor = vec4(1,0,0,1);
	} else {
	    FragColor = colorAcum;
	}*/
	/*if (texture(MeshDepth, EntryCoord.st/ScreenSize).x < 1) {
		FragColor = vec4(1,0,0,1);
	} else {
		FragColor = colorAcum;
	}*/
	/*if ((gl_FragCoord.st / ScreenSize).x - EntryCoord.x == 0) {
		FragColor = vec4(1, 0, 0, 1);
	} else {
		FragColor = vec4(0, (exitPoint - EntryCoord).z, 0, 1);
	}*/
	/*if (texture(MeshDepth, (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0).x < 1) {
		FragColor = vec4(1, 0, 0, 1);
	} else {
		FragColor = vec4(0, 1, 0, 1);
	}*/
	//FragColor = vec4(gl_FragCoord.w * (ExitPointCoord.w), 0, 0, 1);
	FragColor = colorAcum;
	//FragColor = vec4(/*currentDepth * 0.9*/ length(exitPoint - viewPos) * 0.25, 0, 0, 1);
	//FragColor = vec4(texture(MeshColor, gl_FragCoord.st/ScreenSize).xyz, 1);
	    

    // FragColor = (vec4(ambient, 0.0f) + vec4(diffuse, 0.0f) + vec4(specular, 0.0f)) * colorAcum;
	// color = vec4(result, 1.0f);
    // for test
    // FragColor = vec4(EntryCoord, 1.0);
    // FragColor = vec4(exitPoint, 1.0);
   
}
