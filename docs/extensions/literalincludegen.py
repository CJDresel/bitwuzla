import os

from sphinx.directives.code import LiteralInclude

class IncludeFile(LiteralInclude):

    def run(self):
        assert self.arguments

        rel_path= os.path.relpath(self.env.config.lig_build_dir,
                                  start=self.env.config.lig_source_dir)

        filename = os.path.join(rel_path, self.arguments[0])
        if filename:
            self.arguments[0] = filename
        return super().run()

def setup(app):
    app.add_config_value('lig_source_dir', '', 'env')
    app.add_config_value('lig_build_dir', '', 'env')
    app.add_directive('literalincludegen', IncludeFile)
    return {
        'version': '0.1',
        'parallel_read_safe': True,
        'parallel_write_safe': True
    }
