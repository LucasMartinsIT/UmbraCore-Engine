#pragma once
#include <filesystem>

namespace eng
{
	class FileSystem
	{
	public:
		std::filesystem::path GetExecutableFolder() const;
		std::filesystem::path GetAssetFolder() const;
	};
}