import os

def itk_include_string(itk_modules_path):
    include_paths = list()
    for root, dirs, files in os.walk(itk_modules_path):
        for name in dirs:
            if name == 'include':
                include_paths.append(os.path.join(root, name))
    include_string = ';'.join(include_paths)
    print(include_string)

itk_include_string(r"D:\ITK\InsightToolkit-5.2.1\src\Modules")