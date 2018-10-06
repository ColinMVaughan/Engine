#ifndef _ASSET_REGISTRATION_H_
#define _ASSET_REGISTRATION_H_
#include <string>
#include "AssetManager.h"
#include "AssetDetail.h"
//-------------------------------------------------------------------------------------------


namespace Assets
{
	BaseAssetRequestEvent* RequestAsset(AssetManager* manager, std::experimental::filesystem::path assetPath);
	void LoadNewAsset(AssetManager* manager, std::experimental::filesystem::path assetPath);
	void LoadAssetFile(AssetManager& Manager, std::experimental::filesystem::path assetPath);

}

//Registers a class or struct as an asset. Assets of this type will be loaded and stored by the asset manager,
//and can be requested by Components.
//
// NAME = String name that will be associated with the asset. Eg. "Mesh" (including quotes)
// TYPE = the Class type. Eg. Mesh (no quotes)
#define REGISTER_ASSET(NAME, TYPE) \
namespace Assets							\
{											\
	namespace detail						\
	{										\
		namespace							\
		{									\
			template<class T>				\
			class AssetRegistration;		\
											\
			template<>						\
			class AssetRegistration<TYPE>	\
			{								\
				static const ::Assets::detail::AssetRegistryEntry<TYPE>& reg;			\
			};																			\
																						\
			const ::Assets::detail::AssetRegistryEntry<TYPE>&							\
				AssetRegistration<TYPE>::reg =											\
				::Assets::detail::AssetRegistryEntry<TYPE>::Instance(NAME);	\
																						\
		}																				\
	}																					\
}																						\


//Registers an asset File type with a function pointer to the function to be used for loading that file.
//
//EXTENSION = String name of the file extension. Eg. ".obj" or ".png" (including quotes)
//LoadFunction = ...
#define REGISTER_FILETYPE(EXTENSION, LOADER) \
namespace Assets							\
{											\
	namespace detail						\
	{										\
		namespace							\
		{									\
			template<class T>				\
			class FileRegistration;		\
											\
			template<>						\
			class FileRegistration<LOADER>	\
			{								\
				static const ::Assets::detail::FileRegistryEntry<LOADER>& reg;			\
			};																			\
																						\
			const ::Assets::detail::FileRegistryEntry<LOADER>&							\
				FileRegistration<LOADER>::reg =											\
				::Assets::detail::FileRegistryEntry<LOADER>::Instance(EXTENSION);	\
																						\
		}																				\
	}																					\
}		

#endif