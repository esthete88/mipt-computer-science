import os

c = get_config()
c.NbConvertApp.export_format = 'pdf'
c.TemplateExporter.template_path = ['.', os.path.expanduser('/home/esthete/anaconda3/lib/python3.6/site-packages/nbconvert/templates/')]
c.Exporter.template_file = 'mytemplate'
