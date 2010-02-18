#include "AdvancedGosuTextInput.hpp"

using namespace GosuEx;

struct Widgets::AdvancedGosuTextInput::Impl {
		unsigned int maxLength;
		std::wstring chars;
};

Widgets::AdvancedGosuTextInput::AdvancedGosuTextInput():
pimpl(new Impl) {
		pimpl->maxLength = 0;
		pimpl->chars = L"";
}

Widgets::AdvancedGosuTextInput::~AdvancedGosuTextInput() {}

unsigned int Widgets::AdvancedGosuTextInput::maxLength() const {
		return pimpl->maxLength;
}

std::wstring Widgets::AdvancedGosuTextInput::allowedCharacters() const {
		return pimpl->chars;
}

void Widgets::AdvancedGosuTextInput::setMaxLength(unsigned int maxlength) {
		pimpl->maxLength = maxlength;
}

void Widgets::AdvancedGosuTextInput::setAllowedCharacters(const std::wstring& chars) {
		pimpl->chars = chars;
}

std::wstring Widgets::AdvancedGosuTextInput::filter(const std::wstring& textIn) const {
		std::wstring textOut = textIn;
		// Check if the chars are allowed
		if (allowedCharacters().length()) {
				for (std::wstring::iterator it = textOut.begin(); it != textOut.end(); ++it) {
						if (allowedCharacters().find_first_of(*it) == allowedCharacters().npos) {
								it = textOut.erase(it);
								if (it == textOut.end())
										break;
						}
				}
		}
		// Check the length
		if (maxLength() != 0 && text().length() + textOut.length() > maxLength()) return L"";
		return textOut;
}