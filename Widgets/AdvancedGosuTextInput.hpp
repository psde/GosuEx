#ifndef GOSUEX_WIDGETS_ADVANCEDGOSUTEXTINPUT_HPP
#define GOSUEX_WIDGETS_ADVANCEDGOSUTEXTINPUT_HPP

#include <Gosu/TextInput.hpp>
#include <boost/scoped_ptr.hpp>

/**
 * As of 0.7.16, it's possible to define a "filter" function
	* The return value of this function will be added to the string.
 * This allows setMaxLenght and setAllowedChar-hacks.
*/

namespace GosuEx {
		namespace Widgets {
				class AdvancedGosuTextInput : public Gosu::TextInput {
						struct Impl;
						boost::scoped_ptr<Impl> pimpl;
				public:
						AdvancedGosuTextInput();
						virtual ~AdvancedGosuTextInput();
						
						// getFoo
						unsigned int maxLength() const;
						std::wstring allowedCharacters() const;

						// setFoo
						void setMaxLength(unsigned int maxlength);
						void setAllowedCharacters(const std::wstring& chars);

						// Here goes the magic.
						virtual std::wstring filter(const std::wstring& textIn) const;
				};
		}
}

#endif