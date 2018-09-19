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

}


#define REGISTER_ASSET(EXTENSION, NAME, TYPE) \
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
				::Assets::detail::AssetRegistryEntry<TYPE>::Instance(EXTENSION, NAME);	\
																						\
		}																				\
	}																					\
}																						\



#define REGISTER_FILETYPE(EXTENSION, LOADFUNCTION) \
		

#endif