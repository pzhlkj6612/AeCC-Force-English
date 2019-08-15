#pragma once
#include <regex>


namespace VersionFunc {
    enum VERSION {
        VERSION_IS_OLDER,
        VERSION_NO_NEWER,
        VERSION_IS_BETA
    };

    constexpr auto JSON_ContentType = _T("application/json");
    constexpr auto Tag_Name_JSON_Key = _T("tag_name");

    struct FourIntegerVersionNumber {
        int Major = 0;
        int Minor = 0;
        int Patch = 0;
        int Build = 0;
        CString errorInfo;

        bool operator<(const FourIntegerVersionNumber& fivm) const {
            return
                Major < fivm.Major ||
                Minor < fivm.Minor ||
                Patch < fivm.Patch ||
                Build < fivm.Build;
        }

        bool operator>(const FourIntegerVersionNumber& fivm) const {

            return
                Major > fivm.Major ||
                Minor > fivm.Minor ||
                Patch > fivm.Patch ||
                Build > fivm.Build;
        }

        bool operator==(const FourIntegerVersionNumber& fivm) const {
            return !(*this < fivm) && !(*this > fivm);
        }
    };

    bool GetVersionInfo(FourIntegerVersionNumber &fourIntegerVersionNumber);
    bool GetMyVersion(FourIntegerVersionNumber &fivm);
    bool QueryWebpage(LPCTSTR url, CString &webPageContent, CString &errorInfo);
    bool GetLatestVersion(FourIntegerVersionNumber &fourIntegerVersionNumber, CString &result, CString &errorInfo);

    const auto tagname_versionNumber_pattern = std::wregex(_T("(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)"),
        std::regex_constants::ECMAScript | std::regex_constants::icase);
}
