////////////////////////////////////////////////////////////////////////////////
/// @brief variant class for vectors
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triagens GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Achim Brandt
/// @author Copyright 2008-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_VARIANT_VARIANT_VECTOR_H
#define TRIAGENS_VARIANT_VARIANT_VECTOR_H 1

#include "Variant/VariantObject.h"

namespace triagens {
  namespace basics {

// -----------------------------------------------------------------------------
// --SECTION--                                               class VariantVector
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Variants
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief variant class for vectors
////////////////////////////////////////////////////////////////////////////////

    class VariantVector : public VariantObject {
      public:

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                           static public constants
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Variants
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief type of VariantObject
////////////////////////////////////////////////////////////////////////////////

        static ObjectType const TYPE = VARIANT_VECTOR;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Variants
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructs a new vector
////////////////////////////////////////////////////////////////////////////////

        VariantVector ();

////////////////////////////////////////////////////////////////////////////////
/// @brief constructs a new vector
////////////////////////////////////////////////////////////////////////////////

        explicit
        VariantVector (vector<string> const&);

////////////////////////////////////////////////////////////////////////////////
/// @brief destructs a vector
////////////////////////////////////////////////////////////////////////////////

        ~VariantVector ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                             VariantObject methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Variants
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        ObjectType type () const;

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        VariantObject* clone () const;

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        void print (StringBuffer&, size_t indent) const;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Variants
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the values
////////////////////////////////////////////////////////////////////////////////

        vector<VariantObject*> const& getValues () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a value
////////////////////////////////////////////////////////////////////////////////

        void add (VariantObject* value);

////////////////////////////////////////////////////////////////////////////////
/// @brief adds many values
////////////////////////////////////////////////////////////////////////////////

        void add (vector<VariantObject*> const& v);

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a string value
////////////////////////////////////////////////////////////////////////////////

        void add (string const& value);

////////////////////////////////////////////////////////////////////////////////
/// @brief adds many string values
////////////////////////////////////////////////////////////////////////////////

        void add (vector<string> const& v);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Variants
/// @{
////////////////////////////////////////////////////////////////////////////////

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief children
////////////////////////////////////////////////////////////////////////////////

        vector<VariantObject*> _values;
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
