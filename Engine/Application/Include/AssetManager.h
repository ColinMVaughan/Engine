#pragma once
#include <thread>
#include <vector>
#include <string>

class BaseAssetPool
{
public:
	template<typename T>
	virtual T* GetPool() = 0;

	template<typename T>
	virtual void LoadAsset(std::string filePath) = 0;
};

template<typename T>
class AssetPool : public BaseAssetPool
{
public:
	template<typename T>
	virtual T* GetPool() = 0;

	template<typename T>
	virtual void LoadAsset(std::string filePath) = 0;

private:
	std::vector<T> m_Pool;
};

class AssetManager
{
private:

	std::thread m_WorkerThread;
	std::vector<BaseAssetPool*> m_Pools;
};