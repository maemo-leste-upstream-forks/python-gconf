import unittest
import pygtk; pygtk.require("2.0")
import gobject
import ORBit
from common import bonobo
import CORBA
import Bonobo

class BonoboTest(unittest.TestCase):

    def testEventSource(self):
        """tests bonobo.EventSource and listener helper"""
        es = bonobo.EventSource()
        self.event_received = False

        def event_cb(listener, event, value):
            self.assertEqual(event, "test")
            self.assertEqual(value.value(), "xxx")
            self.event_received = True

        def fire_event():
            es.notify_listeners("test", CORBA.Any(CORBA.TC_string, "xxx"))
            bonobo.main_quit()

        bonobo.event_source_client_add_listener(es.corba_objref(), event_cb, "test")
        gobject.idle_add(fire_event)
        bonobo.main()
        self.assert_(self.event_received)

    def testClosure(self):
        """tests pybonobo_closure_new"""
        def enum_objects_cb(handler):
            return ()

        def get_object_cb(handler, item_name, only_if_exists):
            raise Bonobo.ItemContainer.NotFound

        bonobo.activate()
        item_handler = bonobo.ItemHandler(enum_objects_cb, get_object_cb)
        self.assertRaises(Bonobo.ItemContainer.NotFound,
                          item_handler.corba_objref().getObjectByName,
                          "ObjectThatDoesNotExist", False)

if __name__ == '__main__':
    unittest.main()
