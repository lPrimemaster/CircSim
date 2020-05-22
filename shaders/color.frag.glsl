#version 450 core

out vec4 fragColour;

//uniform sampler2D diffuse_sprite;
uniform sampler2DArray diffuse_sprite;
uniform uint sprite_layer; // default 0

uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

uniform int WrapMode;
uniform vec2 SliceBorder;
uniform vec2 SliceSize;



in vec2 uvs;

float map(float value, float originalMin, float originalMax, float newMin, float newMax) 
{
    return (value - originalMin) / (originalMax - originalMin) * (newMax - newMin) + newMin;
}

float sliceUV(float coord, float textureBorder, float windowBorder)
{
	if (coord < windowBorder)
        return map(coord, 0, windowBorder, 0, textureBorder);
    if (coord < 1 - windowBorder) 
        return map(coord,  windowBorder, 1 - windowBorder, textureBorder, 1 - textureBorder);
    return map(coord, 1 - windowBorder, 1, 1 - textureBorder, 1);
}

// @FIX
float sliceUVInverse(float coord, float textureBorder, float windowBorder)
{
	if (coord > windowBorder)
        return map(coord, 0, windowBorder, 0, textureBorder);
    if (coord > 1 - windowBorder) 
        return map(coord,  windowBorder, 1 - windowBorder, textureBorder, 1 - textureBorder);
    if(coord > 1)
    	return map(coord, 1 - windowBorder, 1, 1 - textureBorder, 1);
    return map(coord, 0, windowBorder, 0, textureBorder);
}

void wrapSlice3()
{
	vec2 newUV = vec2(
        sliceUV(uvs.x, SliceBorder.x, SliceSize.x),
        uvs.y
    );

    fragColour = texture(diffuse_sprite, vec3(newUV, sprite_layer));
}

void wrapSlice9()
{
    vec2 newUV = vec2(
        sliceUV(uvs.x, SliceBorder.x, SliceSize.x),
        sliceUV(uvs.y, SliceBorder.y, SliceSize.y)
    );

    fragColour = texture(diffuse_sprite, vec3(newUV, sprite_layer));
}

void wrapBoundary()
{
	vec2 newUV = vec2(
        sliceUVInverse(uvs.x, SliceBorder.x, SliceSize.x),
        uvs.y
    );

    fragColour = texture(diffuse_sprite, vec3(newUV, sprite_layer));
}

void wrapNormal()
{
	fragColour = texture(diffuse_sprite, vec3(uvs, sprite_layer));
}

void main()
{
	if(WrapMode == 0)
	{
		wrapNormal();
	}
	else if(WrapMode == 1)
	{
		wrapSlice3();
	}
	else if(WrapMode == 2)
	{
		wrapSlice9();
	}
	else if(WrapMode == 3)
	{
		wrapBoundary();
	}
}
