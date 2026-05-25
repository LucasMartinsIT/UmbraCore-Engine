#pragma once
#include <filesystem>
#include <vector>

namespace eng
{
	class FileSystem
	{
	public:
		std::filesystem::path GetExecutableFolder() const;
		std::filesystem::path GetAssetFolder() const;

		std::vector<char> LoadFile(const std::filesystem::path& path);
		std::vector<char> LoadAssetFile(const std::string& relativePath);
		std::string LoadAssetsFileText(const std::string& relativePath);
	};
}