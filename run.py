from flask              import Flask, render_template, flash, jsonify, request
from flask_bootstrap    import Bootstrap
from flask_appconfig    import AppConfig
from flask_wtf          import Form, RecaptchaField
from flask_wtf.file     import FileField
from wtforms            import TextField, HiddenField, ValidationError, RadioField,\
                        BooleanField, SubmitField, IntegerField, FormField, validators
from wtforms.validators import Required
#from GPIO               import open_garage

def create_app(configfile=None):
    app = Flask(__name__)
    AppConfig(app, configfile)  # Flask-Appconfig is not necessary, but
                                # highly recommend =)
                                # https://github.com/mbr/flask-appconfig
    Bootstrap(app)

    # in a real app, these should be configured through Flask-Appconfig
    app.config['SECRET_KEY'] = 'devkey'
    app.config['RECAPTCHA_PUBLIC_KEY'] = \
        '6Lfol9cSAAAAADAkodaYl9wvQCwBMr3qGR_PPHcw'

    @app.route('/garage')
    def add_numbers():
        ajax_request = request.args.get('ajax_request', 0, type=str)
        print ("wassup!\nwassup!\nwassup!\nwassup!\nwassup!\nwassup!\nwassup!\n")
        #open_garage()
        return jsonify(result=ajax_request)

    @app.route('/', methods=('GET', 'POST'))
    def index():
        #form = ExampleForm()
        #form.validate_on_submit()  # to get error messages to the browser
        #flash('critical message', 'critical')
        #flash('error message', 'error')

        #flash('info message', 'info')
        #flash('debug message', 'debug')
        #flash('different message', 'different')
        #flash('uncategorized message')
        flash('This application is undergoing tests', 'warning')
        #return render_template('index.html', form=form)
        return render_template('index.html')

    return app

if __name__ == '__main__':
    create_app().run(debug = True, host = '0.0.0.0')
