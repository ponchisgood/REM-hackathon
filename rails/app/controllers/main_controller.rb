class MainController < ApplicationController
    def index
        file = File.read(Rails.public_path.join('pageview.json'))
        data_hash = JSON.parse(file)
        @ads = data_hash['seg'][6]['ads']
        @user_count = data_hash['seg'][6]['count']
    end


    def data
        file = File.read(Rails.public_path.join('pageview.json'))
        respond_to do |format|
            format.json {
                render :json => file
            }
        end
    end
end
