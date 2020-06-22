
#ifndef OPENE57_EXPORT_H
#define OPENE57_EXPORT_H

#ifdef OPENE57_STATIC_DEFINE
#  define OPENE57_EXPORT
#  define OPENE57_NO_EXPORT
#else
#  ifndef OPENE57_EXPORT
#    ifdef openE57_EXPORTS
        /* We are building this library */
#      define OPENE57_EXPORT 
#    else
        /* We are using this library */
#      define OPENE57_EXPORT 
#    endif
#  endif

#  ifndef OPENE57_NO_EXPORT
#    define OPENE57_NO_EXPORT 
#  endif
#endif

#ifndef OPENE57_DEPRECATED
#  define OPENE57_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef OPENE57_DEPRECATED_EXPORT
#  define OPENE57_DEPRECATED_EXPORT OPENE57_EXPORT OPENE57_DEPRECATED
#endif

#ifndef OPENE57_DEPRECATED_NO_EXPORT
#  define OPENE57_DEPRECATED_NO_EXPORT OPENE57_NO_EXPORT OPENE57_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef OPENE57_NO_DEPRECATED
#    define OPENE57_NO_DEPRECATED
#  endif
#endif

#endif /* OPENE57_EXPORT_H */
