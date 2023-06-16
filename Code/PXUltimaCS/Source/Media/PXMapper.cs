using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PX
{
    public class UnmanagedStructure : IDisposable
    {
        protected GCHandle _garbageCollectorHandle;

        public unsafe void* Adress 
        {
#if true
            get => (void*)GCHandle.ToIntPtr(_garbageCollectorHandle);
#else
           // get => (void*)GCHandle.ToIntPtr(_garbageCollectorHandle);
#endif
        }

        public UnmanagedStructure()
        {
            
        }

        public void OwnerSet(object ownerObject)
        {
            _garbageCollectorHandle = GCHandle.Alloc(ownerObject);
        }

        public static unsafe T ObjectFromAdress<T>(void* adress) where T : class
        {
#if false
            GCHandle objectAddress = GCHandle.FromIntPtr((IntPtr)adress);
#else
            GCHandle objectAddress = (GCHandle)(IntPtr)adress;
#endif
            T objectReference = objectAddress.Target as T;
            return objectReference;
        }

        public void Dispose()
        {
            _garbageCollectorHandle.Free();
        }

        public static unsafe void* DelegateToAdress<TDelegate>(TDelegate function)
        {
            return Marshal.GetFunctionPointerForDelegate(function).ToPointer();
        }    
    }
}
