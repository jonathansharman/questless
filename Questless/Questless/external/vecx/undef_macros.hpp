//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Undefines vecx convenience macros, to avoid polluting the global namespace.
//!
//! Include this file at the end of another file, after using the macros.

#pragma once

#undef DEFINE_VECTOR_INDEX_NAME
#undef DEFINE_BOX_SIZE_NAME
#undef DEFINE_BOX_EXTREMES_NAMES
