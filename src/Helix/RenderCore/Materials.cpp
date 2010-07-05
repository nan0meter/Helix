#include "stdafx.h"
#include "Materials.h"

typedef std::map<const std::string, HXMaterial *>	MaterialMap;
struct MaterialState
{
	MaterialMap		m_database;
};
MaterialState	*m_materialState;

// ****************************************************************************
// ****************************************************************************
void HXInitializeMaterials()
{
	m_materialState = new MaterialState;
}

// ****************************************************************************
// ****************************************************************************
HXMaterial * HXCreateMaterial(const std::string &name, LuaObject &object)
{
	HXMaterial * newMat = new HXMaterial;
	newMat->m_name = name;

	// Load our shader
	LuaObject obj = object["Shader"];
	_ASSERT(obj.IsString());
	newMat->m_shaderName = obj.GetString();

	obj = object["Texture"];
	if(!obj.IsNil())
	{
		newMat->m_textureName = obj.GetString();
	}

	// Load the shader
	Helix::Shader *shader = Helix::ShaderManager::GetInstance().Load(newMat->m_shaderName);
	
	// Make sure we can load the associated texture
	// Texture names wrapped in []'s signify a render target or other
	// system texture
	if(newMat->m_textureName.empty() || (newMat->m_textureName[0] == '[' && newMat->m_textureName[newMat->m_textureName.length()-1] == ']') )
		return newMat;

	Helix::Texture *tex = Helix::TextureManager::GetInstance().LoadTexture(newMat->m_textureName);
	_ASSERT(tex != NULL);

	return newMat;
}

// ****************************************************************************
// ****************************************************************************
HXMaterial * HXGetMaterial(const std::string &name)
{
	MaterialMap::const_iterator iter = m_materialState->m_database.find(name);
	if(iter != m_materialState->m_database.end())
		return iter->second;

	return NULL;
}

// ****************************************************************************
// ****************************************************************************
HXMaterial * HXLoadMaterial(const std::string &name)
{
	HXMaterial *mat = HXGetMaterial(name);
	if(mat != NULL)
		return mat;

	std::string fullPath = "Materials/";
	fullPath += name;
	fullPath += ".lua";

	LuaState *state = LuaState::Create();
	_ASSERT(state != NULL);
	
	int retVal = state->DoFile(fullPath.c_str());
	_ASSERT(retVal == 0);

	LuaObject shaderObj = state->GetGlobals()["Material"];
	_ASSERT(shaderObj.IsTable());

	mat = HXCreateMaterial(name,shaderObj);
	_ASSERT(mat != NULL);

	m_materialState->m_database[name] = mat;
	return mat;
}