// <class>.hpp 
// Include file for the <class> class.
// Revision History:
// 05/07/24 Joshua Archibald Initial Revision.

// Included libraries.
#include <stdlib.h>

#ifndef CLASS_HPP
#define CLASSO_HPP

class code_macro {
	// Publicly usable.
	public:
		// Constructor.
		<class>();
		<class>(size_t arg);
		
		// Destructor.
		~<lass>();

		// Public Methods
		void <class method> (size_t arg);

	// Private usage only.
	private:
		// Private data members.
		size_t data;

		// Private helper functions.
		void <class method> (size_t arg);
};

#endif // CLASS_HPP
