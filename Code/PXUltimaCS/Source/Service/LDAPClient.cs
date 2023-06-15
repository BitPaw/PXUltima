using System;

namespace PX
{
    public class LDAPClient : IDisposable
    {
        public LDAPClient()
        {

        }

        public PX.ActionResult Open(bool connectionOriented, string host, int port, bool useSSL, int timeout, string ConnectionDomain, string authenticationCredentials, int authenticationMethod)
        {
            return ActionResult.NotImplemented;
        }

        public PX.ActionResult Close() 
        {
            return ActionResult.NotImplemented; 
        }

        public PX.ActionResult Search(string entryName, string filter, int scope, bool onlyTypesRequired, bool async)
        {
            return ActionResult.NotImplemented;
        }

        public void Dispose()
        {
           
        }
    }
}
