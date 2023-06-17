using System;
using System.Reflection;

namespace PX
{
	public class Serializer
	{
		public static PX.ActionResult FromObject<T>(T obj)
		{
			FieldInfo[] fielList = typeof(T).GetFields();

			if (fielList.Length == 0) return ActionResult.Invalid; // No fields, maybe they are all not set public

			foreach (FieldInfo fieldInfo in fielList)
			{       
                object fieldData = fieldInfo.GetValue(obj);
                System.Type type = fieldData.GetType();
				int size = -1;

                Console.WriteLine(type.Name + " " + fieldInfo.Name + " : " + fieldData + " " + size + " Bytes");
            }

            return ActionResult.NotImplemented;
        }

		public static PX.ActionResult ToObject<T>(T obj)
		{
			return ActionResult.NotImplemented;
		}
	}
}