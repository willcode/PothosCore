//
// Exception.hpp
//
// Definition of various Pothos exception classes.
//
// Copyright (c) 2013-2014 Josh Blum
// SPDX-License-Identifier: BSL-1.0
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#pragma once
#include <Pothos/Config.hpp>
#include <stdexcept>
#include <string>

namespace Pothos {


class POTHOS_API Exception: public std::exception
	/// This is the base class for all exceptions defined
	/// in the Pothos class library.
{
public:
	Exception(const std::string& msg, int code = 0);
		/// Creates an exception.

	Exception(const std::string& msg, const std::string& arg, int code = 0);
		/// Creates an exception.

	Exception(const std::string& msg, const Exception& nested, int code = 0);
		/// Creates an exception and stores a clone
		/// of the nested exception.

	Exception(const Exception& exc);
		/// Copy constructor.
		
	~Exception() throw();
		/// Destroys the exception and deletes the nested exception.

	Exception& operator = (const Exception& exc);
		/// Assignment operator.

	virtual const char* name() const throw();
		/// Returns a static string describing the exception.
		
	virtual const char* className() const throw();
		/// Returns the name of the exception class.
		
	virtual const char* what() const throw();
		/// Returns a static string describing the exception.
		///
		/// Same as name(), but for compatibility with std::exception.
		
	const Exception* nested() const;
		/// Returns a pointer to the nested exception, or
		/// null if no nested exception exists.
			
	const std::string& message() const;
		/// Returns the message text.
			
	int code() const;
		/// Returns the exception code if defined.
		
	std::string displayText() const;
		/// Returns a string consisting of the
		/// message name and the message text.

	virtual Exception* clone() const;
		/// Creates an exact copy of the exception.
		///
		/// The copy can later be thrown again by
		/// invoking rethrow() on it.
		
	virtual void rethrow() const;
		/// (Re)Throws the exception.
		///
		/// This is useful for temporarily storing a
		/// copy of an exception (see clone()), then
		/// throwing it again.

protected:
	Exception(int code = 0);
		/// Standard constructor.

	void message(const std::string& msg);
		/// Sets the message for the exception.

	void extendedMessage(const std::string& arg);
		/// Sets the extended message for the exception.
		
private:
	std::string _msg;
	Exception*  _pNested;
	int			_code;
};


//
// inlines
//
inline const Exception* Exception::nested() const
{
	return _pNested;
}


inline const std::string& Exception::message() const
{
	return _msg;
}


inline void Exception::message(const std::string& msg)
{
	_msg = msg;
}


inline int Exception::code() const
{
	return _code;
}


//
// Macros for quickly declaring and implementing exception classes.
// Unfortunately, we cannot use a template here because character
// pointers (which we need for specifying the exception name)
// are not allowed as template arguments.
//
#define POTHOS_DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE) \
	class API CLS: public BASE														\
	{																				\
	public:																			\
		CLS(int code = CODE);														\
		CLS(const std::string& msg, int code = CODE);								\
		CLS(const std::string& msg, const std::string& arg, int code = CODE);		\
		CLS(const std::string& msg, const Pothos::Exception& exc, int code = CODE);	\
		CLS(const CLS& exc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& exc);											\
		const char* name() const throw();											\
		const char* className() const throw();										\
		Pothos::Exception* clone() const;												\
		void rethrow() const;														\
	};

#define POTHOS_DECLARE_EXCEPTION(API, CLS, BASE) \
	POTHOS_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)

#define POTHOS_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int code): BASE(code)																	\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, int code): BASE(msg, code)										\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)		\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const Pothos::Exception& exc, int code): BASE(msg, exc, code)	\
	{																								\
	}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
	{																								\
	}																								\
	CLS::~CLS() throw()																				\
	{																								\
	}																								\
	CLS& CLS::operator = (const CLS& exc)															\
	{																								\
		BASE::operator = (exc);																		\
		return *this;																				\
	}																								\
	const char* CLS::name() const throw()															\
	{																								\
		return NAME;																				\
	}																								\
	const char* CLS::className() const throw()														\
	{																								\
		return typeid(*this).name();																\
	}																								\
	Pothos::Exception* CLS::clone() const																\
	{																								\
		return new CLS(*this);																		\
	}																								\
	void CLS::rethrow() const																		\
	{																								\
		throw *this;																				\
	}


//
// Standard exception classes
//
POTHOS_DECLARE_EXCEPTION(POTHOS_API, LogicException, Exception)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, AssertionViolationException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, NullPointerException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, NullValueException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, BugcheckException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, InvalidArgumentException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, NotImplementedException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, RangeException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, IllegalStateException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, InvalidAccessException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, SignalException, LogicException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, UnhandledException, LogicException)

POTHOS_DECLARE_EXCEPTION(POTHOS_API, RuntimeException, Exception)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, NotFoundException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, ExistsException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, TimeoutException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, SystemException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, RegularExpressionException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, LibraryLoadException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, LibraryAlreadyLoadedException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, NoThreadAvailableException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, PropertyNotSupportedException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, PoolOverflowException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, NoPermissionException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, OutOfMemoryException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, DataException, RuntimeException)

POTHOS_DECLARE_EXCEPTION(POTHOS_API, DataFormatException, DataException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, SyntaxException, DataException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, CircularReferenceException, DataException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, PathSyntaxException, SyntaxException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, IOException, RuntimeException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, ProtocolException, IOException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, FileException, IOException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, FileExistsException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, FileNotFoundException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, PathNotFoundException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, FileReadOnlyException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, FileAccessDeniedException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, CreateFileException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, OpenFileException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, WriteFileException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, ReadFileException, FileException)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, UnknownURISchemeException, RuntimeException)

POTHOS_DECLARE_EXCEPTION(POTHOS_API, ApplicationException, Exception)
POTHOS_DECLARE_EXCEPTION(POTHOS_API, BadCastException, RuntimeException)


} // namespace Pothos
