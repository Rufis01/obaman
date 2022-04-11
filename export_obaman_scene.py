import bpy
import os
import numpy
import math
from fxpmath import Fxp
from bpy_extras.io_utils import axis_conversion

OSF_SCALE = 1000.0


def export_obj(o, filepath):
    zero = [0] * 3
    loc = o.location.copy()
    rot = o.rotation_euler.copy()
    o.location = zero
    o.rotation_euler = zero
    o.select_set(True)
    bpy.ops.export_scene.obj(filepath=filepath, axis_forward='Z', axis_up='-Y', use_smooth_groups=False, use_materials=False, use_triangles=True, use_blen_objects=False, use_selection=True)
    o.select_set(False)
    o.location = loc.copy()
    o.rotation_euler = rot.copy()

def export_scene(context, filepath, exp_obj):
    
    basedir, fname = os.path.split(os.path.abspath(filepath))
    filepath = basedir + '\\' + fname.upper()
    
    conv_mat = axis_conversion(to_forward='Z', to_up='-Y').to_4x4()
    conv_irot = axis_conversion(from_forward='Z', from_up='-Y').to_4x4()
    
    f = open(filepath, 'wb')
    
    cam = bpy.context.scene.camera
    cam.matrix_world = conv_mat @ cam.matrix_world
    
    f.write(bytes('c', 'ASCII'))
    f.write(numpy.int32(Fxp((cam.location[0] * OSF_SCALE), signed=True, n_word=32, n_frac=12).val).tobytes())
    f.write(numpy.int32(Fxp((cam.location[1] * OSF_SCALE), signed=True, n_word=32, n_frac=12).val).tobytes())
    f.write(numpy.int32(Fxp((cam.location[2] * OSF_SCALE), signed=True, n_word=32, n_frac=12).val).tobytes())
        
    cam.matrix_world = conv_irot @ cam.matrix_world
    
    
    f.write(numpy.int16(((90-math.degrees(cam.rotation_euler[0]))*4096.0)/360.0).tobytes())
    f.write(numpy.int16((math.degrees(cam.rotation_euler[1])*4096.0)/360.0).tobytes())
    f.write(numpy.int16((math.degrees(-cam.rotation_euler[2])*4096.0)/360.0).tobytes())
    
    f.write(b'\0\0') #Add padding for SVECTOR
        
    f.write(numpy.int32(2848194).tobytes())    #FOV. Code is buggy (it is ignored anyways when loaded)
    
    
    bpy.ops.object.select_all(action='DESELECT')
    
    for o in bpy.context.scene.collection.all_objects:
        if o.type == 'MESH':
            name = bpy.path.display_name_to_filepath(o.name).upper()
            path = basedir + '\\' + name
            if(exp_obj):
                export_obj(o, path + '.OBJ')
            f.write(bytes('o', 'ASCII'))
            
            o.matrix_world = conv_mat @ o.matrix_world
            for i in range(3):
                f.write(numpy.int32(Fxp((o.location[i] * OSF_SCALE), signed=True, n_word=32, n_frac=12).val).tobytes())
            o.matrix_world = conv_irot @ o.matrix_world
            
            f.write(numpy.int16((math.degrees(o.rotation_euler[0])*4096.0)/360.0).tobytes())
            f.write(numpy.int16((math.degrees(-o.rotation_euler[2])*4096.0)/360.0).tobytes())
            f.write(numpy.int16((math.degrees(o.rotation_euler[1])*4096.0)/360.0).tobytes())
            f.write(b'\0\0') #Add padding for SVECTOR
    
            f.write(bytes(name + '.IDX', 'ASCII') + b'\0')
            #We're missing position and rotation data here ;-;
    
    f.close()
    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportObamanScene(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_scene.obaman"
    bl_label = "Export Obaman Scene"

    # ExportHelper mixin class uses this
    filename_ext = ".osf"

    filter_glob: StringProperty(
        default="*.osf",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.

    exp_obj: BoolProperty(
        name="Export OBJs",
        description="Export OBJs for each model",
        default=False,
    )

    def execute(self, context):
        return export_scene(context, self.filepath, self.exp_obj)

# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportObamanScene.bl_idname, text="Obaman Scene")

# Register and add to the "file selector" menu (required to use F3 search "Text Export Operator" for quick access)
def register():
    bpy.utils.register_class(ExportObamanScene)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportObamanScene)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
